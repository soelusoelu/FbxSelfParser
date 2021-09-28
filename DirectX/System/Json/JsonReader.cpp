#include "JsonReader.h"
#include "JsonStream.h"
#include <cassert>

JsonReader::JsonReader() = default;

JsonReader::~JsonReader() = default;

void JsonReader::parse(JsonStream& in, JsonObject& value) const {
    //中身の先頭までスキップ
    skipSpaceAndComments(in);

    //中身が空ならエラー
    if (in.peek() == '\0') {
        assert(false);
    } else {
        //要素を解析していく
        parseObject(in, value, "");

        skipSpaceAndComments(in);

        //解析後ヌル文字で終わっていなければエラー
        if (in.peek() != '\0') {
            assert(false);
        }
    }
}

void JsonReader::parseValue(JsonStream& in, JsonObject& value, const std::string& key) const {
    char c = in.peek();
    if (c == '{') {
        auto out = std::make_shared<JsonObject>();
        parseObject(in, *out, key);
        value.children.emplace(key, out);
    } else if (c == '"') {
        parseString(in, value.values.emplace(key, std::string()).first->second);
    } else if (c == '[') {
        parseArray(in, value, key);
    } else {
        parseNumber(in, value.values.emplace(key, std::string()).first->second);
    }
}

void JsonReader::parseObject(JsonStream& in, JsonObject& value, const std::string& key) const {
    assert(in.peek() == '{');
    in.take(); //skip {
    skipSpaceAndComments(in);

    //空オブジェクトかチェック
    if (consume(in, '}')) {
        return;
    }

    while (true) {
        std::string name;
        parseString(in, name);

        skipSpaceAndComments(in);

        assert(in.peek() == ':');
        in.take(); //skip :

        skipSpaceAndComments(in);

        parseValue(in, value, name);

        skipSpaceAndComments(in);

        if (consume(in, ',')) {
            skipSpaceAndComments(in);
        } else if (consume(in, '}')) {
            return;
        }
    }
}

void JsonReader::parseNumber(JsonStream& in, std::string& out) const {
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
        } else if (consume(in, '-')) {
            out += '-';
        }

        c = in.peek();
        while ((c >= '0' && c <= '9') || c == '.') {
            out += c;
            c = in.take();
        }
    }
}

void JsonReader::parseString(JsonStream& in, std::string& out) const {
    assert(in.peek() == '"');

    char c = in.take();
    while (c != '"') {
        out += c;
        c = in.take();
    }

    in.take(); //skip "
}

void JsonReader::parseArray(JsonStream& in, JsonObject& out, const std::string& key) const {
    assert(in.peek() == '[');
    in.take(); //skip [
    skipSpaceAndComments(in);

    //空配列かチェック
    if (consume(in, ']')) {
        return;
    }

    auto c = in.peek();
    if (c == '"') {
        auto& target = out.valueArray.emplace(key, JsonValueArray{}).first->second;
        parseString(in, target.emplace_back());
    } else if (c == '{') {
        auto target = std::make_shared<JsonObject>();
        parseObject(in, *target, std::string());
        JsonObjectArray targetArray(1, target);
        out.arrayChildren.emplace(key, targetArray);
    } else {
        auto& target = out.valueArray.emplace(key, JsonValueArray{}).first->second;
        parseNumber(in, target.emplace_back());
    }

    skipSpaceAndComments(in);

    if (consume(in, ',')) {
        skipSpaceAndComments(in);
    } else if (consume(in, ']')) {
        return;
    } else {
        assert(false);
    }

    while (true) {
        c = in.peek();
        if (c == '"') {
            parseString(in, out.valueArray[key].emplace_back());
        } else if (c == '{') {
            auto target = std::make_shared<JsonObject>();
            parseObject(in, *target, std::string());
            out.arrayChildren[key].emplace_back(target);
        } else {
            parseNumber(in, out.valueArray[key].emplace_back());
        }

        skipSpaceAndComments(in);

        if (consume(in, ',')) {
            skipSpaceAndComments(in);
        } else if (consume(in, ']')) {
            break;
        } else {
            assert(false);
        }
    }
}

void JsonReader::skipSpaceAndComments(JsonStream& in) const {
    skipSpace(in);

    //スラッシュならコメント
    while (consume(in, '/')) {
        //次がアスタリスクなら範囲コメント
        if (consume(in, '*')) {
            while (true) {
                if (in.peek() == '\0') {
                    assert(false);
                } else if (consume(in, '*')) {
                    if (consume(in, '/')) {
                        break;
                    }
                } else {
                    in.take();
                }
            }
        }
        //スラッシュ2個続き(行コメント)
        else if (consume(in, '/')) {
            while (in.peek() != '\0' && in.take() != '\n') {}
        }
        //上記以外はありえない
        else {
            assert(false);
        }

        skipSpace(in);

        //コメントが続く限り続ける
    }
}

void JsonReader::skipSpace(JsonStream& in) const {
    //スペース、改行、リターン、タブをスキップする
    char c = in.peek();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        c = in.take();
    }
}

bool JsonReader::consume(JsonStream& in, char expect) const {
    if (in.peek() == expect) {
        in.take();
        return true;
    } else {
        return false;
    }
}
