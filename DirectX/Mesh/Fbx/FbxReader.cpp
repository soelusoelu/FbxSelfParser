#include "FbxReader.h"
#include "FbxStream.h"
#include "FbxValue.h"
#include <assert.h>

void FbxReader::parse(FbxStream& in, FbxValue& value) const {
    //中身の先頭までスキップ
    skipSpaceAndComments(in);

    //中身が空ならエラー
    if (in.peek() == '\0') {
        assert(false);
    } else {
        //全ての要素を解析していく
        while (in.peek() != '\0') {
            //auto& v = value.values.emplace_back();
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

void FbxReader::parseAttributesOrValue(FbxStream& in, FbxValue& value, const std::string& key) const {
    std::string s;
    char c = in.peek();
    if (c == '{') {
        auto& v = value.children.emplace_back();
        v.name = key;
        parseObject(in, v);
        return;
    } else if (c == '"') {
        parseString(in, s);
    } else {
        parseNumber(in, s);
    }

    skipSpaceAndComments(in);

    c = in.peek();
    if (c == ',') {
        auto& back = value.children.back();
        if (back.name.empty() && back.values.empty() && back.children.empty()) {
            back.attributes.emplace_back(s);
        } else {
            auto& v = value.children.emplace_back();
            v.attributes.emplace_back(s);
        }

        in.take(); //skip ,
        skipSpaceAndComments(in);

        parseAttributesOrValue(in, value, key);
    } else if (c == '{') {
        auto& back = value.children.back();
        if (back.name.empty() && back.values.empty() && back.children.empty()) {
            back.attributes.emplace_back(s);
        } else {
            auto& v = value.children.emplace_back();
            v.attributes.emplace_back(s);
        }

        auto& v = value.children.back();
        v.name = key;
        parseObject(in, v);
    } else {
        value.values.emplace(key, s);
    }
}

void FbxReader::parseAttributes(FbxStream& in, FbxValue& value, const std::string& key) const {
    std::string s;
    char c = in.peek();
    if (c == '{') {
        parseObject(in, value);
        return;
    } else if (c == '"') {
        parseString(in, s);
    } else {
        parseNumber(in, s);
    }

    skipSpaceAndComments(in);

    c = in.peek();
    if (c == ',') {
        value.attributes.emplace_back(s);

        skipSpaceAndComments(in);

        parseAttributes(in, value, key);
    } else if (c == '{') {
        value.attributes.emplace_back(s);

        skipSpaceAndComments(in);

        value.attributes.emplace_back(s);
    } else {
        value.values.emplace(key, s);
    }
}

void FbxReader::parseObject(FbxStream& in, FbxValue& out) const {
    assert(in.peek() == '{');
    in.take(); //skip {
    skipSpaceAndComments(in);

    //空オブジェクトかチェック
    if (consume(in, '}')) {
        return;
    }

    unsigned memberCount = 0;
    while (true) {
        std::string name;
        parseValueName(in, name);
        assert(in.peek() == ':');
        in.take(); //skip :

        skipSpaceAndComments(in);

        parseAttributesOrValue(in, out, name);

        skipSpaceAndComments(in);

        ++memberCount;

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
    assert(in.peek() >= '0' && in.peek() <= '9');

    char c = in.peek();
    while (c >= '0' && c <= '9') {
        out += c;
        c = in.take();
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

bool FbxReader::consume(FbxStream& in, const char expect) const {
    if (static_cast<char>(in.peek()) == expect) {
        in.take();
        return true;
    } else {
        return false;
    }
}
