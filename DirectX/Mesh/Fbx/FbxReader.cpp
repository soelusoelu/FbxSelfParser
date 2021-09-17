#include "FbxReader.h"
#include "FbxStream.h"
#include "FbxObject.h"
#include <assert.h>

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

            parseAttributesOrValue(in, value, name);

            skipSpaceAndComments(in);
        }
    }
}

void FbxReader::parseAttributesOrValue(FbxStream& in, FbxObject& value, const std::string& key) const {
    std::string s;
    char c = in.peek();
    if (c == '{') {
        auto& v = value.children.emplace_back();
        v.name = key;
        parseObject(in, v);
        return;
    } else if (c == '"') {
        parseString(in, s);
    } else if (c == '*') {
        parseArray(in, value, key);
        return;
    } else {
        parseNumberArray(in, s);
    }

    skipSpaceAndComments(in);

    c = in.peek();
    //attributeが続くか、数値が続いている
    if (c == ',') {
        getObjectAddAttribute(value).attributes.emplace_back(s);

        in.take(); //skip ,
        skipSpace(in);

        parseAttributesOrValue(in, value, key);
    } 
    //attributeの終わり
    else if (c == '{') {
        auto& v = getObjectAddAttribute(value);

        v.attributes.emplace_back(s);
        v.name = key;

        parseObject(in, v);
    } 
    //attributeではない
    else {
        value.values.emplace(key, s);
    }

    value.values.emplace(key, s);
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
        parseNumberArray(in, value);
    }

    skipSpaceAndComments(in);
}

void FbxReader::parseObject(FbxStream& in, FbxObject& out) const {
    assert(in.peek() == '{');
    in.take(); //skip {
    skipSpaceAndComments(in);

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
            parseProperties70(in, out);
        } else {
            parseAttributesOrValue(in, out, name);
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
}

void FbxReader::parseNumberArray(FbxStream& in, std::string& out) const {
    parseNumber(in, out);

    skipSpace(in);
    while (in.peek() == ',') {
        auto pos = in.tell();
        in.take(); //skip ,
        skipSpace(in);

        char c = in.peek();
        if ((c >= '0' && c <= '9') || c == '-') {
            out += ',';
            parseNumber(in, out);
        } else {
            in.seek(pos);
            break;
        }
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

    //空配列かチェック
    if (consume(in, '}')) {
        return;
    }

    skipSpace(in);

    //文字列から要素数を取得する
    unsigned numValue = static_cast<unsigned>(std::stoi(numValueStr));

    FbxArrayValue values(numValue);

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

    out.arrayValues.emplace(key, values);
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

        parseProperties70Value(in, out.properties.emplace_back());

        c = in.peek();
    }

    in.take(); //skip }
}

void FbxReader::parseProperties70Value(FbxStream& in, FbxProperties70& out) const {
    parseString(in, out.name);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    parseString(in, out.type);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    parseString(in, out.type2);
    skipSpace(in);
    assert(in.peek() == ',');
    in.take(); //skip ,
    skipSpace(in);

    parseString(in, out.unknown);
    skipSpace(in);

    //5つ目の値がないこともあるためチェック
    if (in.peek() != ',') {
        return;
    }

    //Vector3など値が一つではないこともあるので、カンマが続く限り文字列を繋げていく
    while (in.peek() == ',') {
        in.take(); //skip ,
        skipSpace(in);

        parseValue(in, out.value);
    }
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
    if (static_cast<char>(in.peek()) == expect) {
        in.take();
        return true;
    } else {
        return false;
    }
}

FbxObject& FbxReader::getObjectAddAttribute(FbxObject& value) const {
    if (value.children.empty()) {
        return value.children.emplace_back();
    }

    auto& back = value.children.back();
    if (back.name.empty() && back.values.empty() && back.children.empty()) {
        return back;
    } else {
        return value.children.emplace_back();
    }
}
