#include "FbxReader.h"
#include "FbxStream.h"
#include "FbxObject.h"
#include <assert.h>

FbxReader::FbxReader() = default;

FbxReader::~FbxReader() = default;

void FbxReader::parse(FbxStream& in, FbxObject& value) const {
    //中身の先頭までスキップ
    skipSpaceAndComments(in);

    //中身が空ならエラー
    if (in.peek() == '\0') {
        assert(false);
    } else {
        //全ての要素を解析していく
        while (in.peek() != '\0') {
            std::string name;
            parseValueName(in, name);
            assert(in.peek() == ':');
            in.take(); //skip :
            skipSpace(in);

            parseAttributesOrValues(in, value, name);

            skipSpaceAndComments(in);
        }
    }
}

void FbxReader::parseAttributesOrValues(FbxStream& in, FbxObject& value, const std::string& key) const {
    std::string s;
    char c = in.peek();
    if (c == '{') {
        parseObject(in, value, key, nullptr);
        return;
    } else if (c == '"') {
        parseString(in, s);
    } else if (c == '*') {
        parseArray(in, value, key);
        return;
    } else {
        parseNumber(in, s);
    }

    skipSpaceAndComments(in);

    //attributeが続くか、値が続いている
    if (in.peek() == ',') {
        std::vector<std::string> values(1, s);

        do {
            in.take(); //skip ,
            skipSpace(in);

            parseValue(in, values.emplace_back());
        } while (in.peek() == ',');

        //値配列の終わり
        if (in.peek() != '{') {
            value.valueArray.emplace(key, values);
        }
        //attributeの終わり
        else {
            parseObject(in, value, key, &values);
        }

        return;
    }
    //attributeの終わり
    else if (in.peek() == '{') {
        std::vector<std::string> tmp(1, s);
        parseObject(in, value, key, &tmp);
    } 
    //attributeではない(値である)
    else {
        value.values.emplace(key, s);
    }
}

void FbxReader::parseValue(FbxStream& in, std::string& value) const {
    char c = in.peek();
    if (c == '{') {
        assert(false);
    } else if (c == '"') {
        parseString(in, value);
    } else if (c == '*') {
        assert(false);
    } else {
        parseNumber(in, value);
    }

    skipSpaceAndComments(in);
}

void FbxReader::parseObject(FbxStream& in, FbxObject& parent, const std::string& name, const std::vector<std::string>* attributes) const {
    assert(in.peek() == '{');
    in.take(); //skip {
    skipSpaceAndComments(in);

    //子オブジェクトを作成する
    auto& child = parent.children.emplace(name, FbxObject{})->second;
    if (attributes) {
        child.attributes = *attributes;
    }

    //空オブジェクトかチェック
    if (consume(in, '}')) {
        return;
    }

    while (true) {
        std::string name;
        parseValueName(in, name);
        assert(in.peek() == ':');
        in.take(); //skip :

        skipSpaceAndComments(in);

        if (name == "Properties70") {
            parseProperties70(in, child);
        } else if (name == "C") {
            parseConnections(in, child.connections.emplace_back());
        } else {
            parseAttributesOrValues(in, child, name);
        }

        skipSpaceAndComments(in);

        if (consume(in, '}')) {
            return;
        }
    }
}

void FbxReader::parseValueName(FbxStream& in, std::string& out) const {
    char c = in.peek();
    while (c != ':') {
        out += c;
        c = in.take();
    }
}

void FbxReader::parseNumber(FbxStream& in, std::string& out) const {
    if (consume(in, '-')) {
        out += '-';
    }

    char c = in.peek();
    while ((c >= '0' && c <= '9') || c == '.') {
        out += c;
        c = in.take();
    }

    if (c == 'e') {
        out += 'e';
        in.take(); //skip e

        if (consume(in, '+')) {
            out += '+';
        }

        parseNumber(in, out);
    }
}

void FbxReader::parseString(FbxStream& in, std::string& out) const {
    assert(in.peek() == '"');

    char c = in.take();
    while (c != '"') {
        out += c;
        c = in.take();
    }

    in.take(); //skip "
}

void FbxReader::parseArray(FbxStream& in, FbxObject& out, const std::string& key) const {
    assert(in.peek() == '*');
    in.take(); //skip *

    skipSpace(in);

    //要素数を文字列で取得する
    std::string numValueStr;
    parseNumber(in, numValueStr);

    skipSpace(in);

    assert(in.peek() == '{');
    in.take(); //skip {

    skipSpace(in);

    //空配列かチェック
    if (consume(in, '}')) {
        return;
    }

    //文字列から要素数を取得する
    unsigned numValue = static_cast<unsigned>(std::stoi(numValueStr));

    FbxValueArray values(numValue);

    assert(in.peek() == 'a');
    in.take(); //skip a
    assert(in.peek() == ':');
    in.take(); //skip :

    skipSpace(in);

    for (unsigned i = 0; i < numValue; ++i) {
        parseNumber(in, values[i]);

        skipSpace(in);

        if (consume(in, ',')) {
            skipSpace(in);
        }
    }

    assert(in.peek() == '}');
    in.take(); //skip }

    out.valueArray.emplace(key, values);
}

void FbxReader::parseProperties70(FbxStream& in, FbxObject& out) const {
    assert(in.peek() == '{');
    in.take(); //skip {

    skipSpaceAndComments(in);

    char c = in.peek();
    while (c != '}') {
        assert(c == 'P');
        c = in.take(); //skip P
        assert(c == ':');
        c = in.take(); //skip :

        skipSpace(in);

        parseProperties70Value(in, out.properties);

        c = in.peek();
    }

    in.take(); //skip }
}

void FbxReader::parseProperties70Value(FbxStream& in, FbxProperties70& out) const {
    std::string name;
    parseString(in, name);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    std::string type;
    parseString(in, type);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    std::string type2;
    parseString(in, type2);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    std::string unknown;
    parseString(in, unknown);
    skipSpace(in);

    //5つ目の値がないこともあるためチェック
    if (in.peek() != ',') {
        out.emplace(name, FbxProperties{ type, type2, unknown, "" });
        return;
    }

    //Vector3など値が一つではないこともあるので、カンマが続く限り文字列を繋げていく
    std::string value;
    while (true) {
        in.take(); //skip ,
        skipSpace(in);

        parseValue(in, value);

        if (in.peek() == ',') {
            //std::istringstreamで読み取る前提でスペースを入れる
            value += ' ';
        } else {
            break;
        }
    }

    out.emplace(name, FbxProperties{ type, type2, unknown, value });
}

void FbxReader::parseConnections(FbxStream& in, FbxConnections& out) const {
    parseString(in, out.related);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    std::string tmp;
    parseNumber(in, tmp);
    out.child = std::stoi(tmp);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    tmp.clear();
    parseNumber(in, tmp);
    out.parent = std::stoi(tmp);
    skipSpace(in);

    //4つ目の値がないこともあるためチェック
    if (in.peek() != ',') {
        return;
    }

    in.take(); //skip ,
    skipSpace(in);

    parseString(in, out.value);
}

void FbxReader::skipSpaceAndComments(FbxStream& in) const {
    skipSpace(in);

    //セミコロンがコメント
    while (consume(in, ';')) {
        while (in.peek() != '\0' && in.take() != '\n') {}

        skipSpace(in);
    }
}

void FbxReader::skipSpace(FbxStream& in) const {
    //スペース、改行、リターン、タブをスキップする
    char c = in.peek();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        c = in.take();
    }
}

bool FbxReader::consume(FbxStream& in, char expect) const {
    if (in.peek() == expect) {
        in.take();
        return true;
    } else {
        return false;
    }
}
