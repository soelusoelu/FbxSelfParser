#include "JsonReader.h"
#include "JsonInputStream.h"
#include "JsonObject.h"
#include "JsonValue.h"
#include <cassert>

JsonReader::JsonReader() = default;

JsonReader::~JsonReader() = default;

void JsonReader::parse(JsonInputStream& in, JsonObject& value) const {
    //中身の先頭までスキップ
    skipSpaceAndComments(in);

    //中身が空ならエラー
    if (in.peek() == '\0') {
        assert(false);
    } else {
        //要素を解析していく
        parseObject(in, value);

        skipSpaceAndComments(in);

        //解析後ヌル文字で終わっていなければエラー
        if (in.peek() != '\0') {
            assert(false);
        }
    }
}

void JsonReader::parseValue(JsonInputStream& in, JsonValue& value) const {
    char c = in.peek();
    if (c == 't') {
        parseTrue(in, value);
    } else if (c == 'f') {
        parseFalse(in, value);
    } else if (c == '{') {
        value.setObject();
        parseObject(in, *value.o);
    } else if (c == '"') {
        parseString(in, value);
    } else if (c == '[') {
        parseArray(in, value);
    } else {
        parseNumber(in, value);
    }
}

void JsonReader::parseObject(JsonInputStream& in, JsonObject& value) const {
    assert(in.peek() == '{');
    in.take(); //skip {
    skipSpaceAndComments(in);

    //空オブジェクトかチェック
    if (consume(in, '}')) {
        return;
    }

    while (true) {
        auto name = parseString(in);

        auto v = std::make_shared<JsonValue>();
        value.values.emplace(name, v);
        value.keys.emplace_back(name);

        skipSpaceAndComments(in);

        assert(in.peek() == ':');
        in.take(); //skip :

        skipSpaceAndComments(in);

        parseValue(in, *v);

        skipSpaceAndComments(in);

        if (consume(in, ',')) {
            skipSpaceAndComments(in);
        } else if (consume(in, '}')) {
            return;
        }
    }
}

void JsonReader::parseNumber(JsonInputStream& in, JsonValue& value) const {
    //文字列で取得してから数値に変換するための一時変数
    std::string num;

    //マイナスか
    if (consume(in, '-')) {
        num += '-';
    }

    //整数部分を取得する
    char c = in.peek();
    while (c >= '0' && c <= '9') {
        num += c;
        c = in.take();
    }

    //整数部分の次がドットじゃなければintで確定
    if (c != '.') {
        //intで登録
        value.setInt(std::stoi(num));
        return;
    }

    //以下小数部分を取得する
    num += '.';
    c = in.take(); //skip .
    while (c >= '0' && c <= '9') {
        num += c;
        c = in.take();
    }

    if (c == 'e') {
        num += 'e';
        in.take(); //skip e

        if (consume(in, '-')) {
            num += '-';
        } else if (consume(in, '+')) {
            num += '+';
        }

        c = in.peek();
        while (c >= '0' && c <= '9') {
            num += c;
            c = in.take();
        }
    }

    //floatで登録
    value.setFloat(std::stof(num));
}

void JsonReader::parseString(JsonInputStream& in, JsonValue& value) const {
    value.setString(parseString(in));
}

std::string JsonReader::parseString(JsonInputStream& in) const {
    assert(in.peek() == '"');

    std::string result;
    char c = in.take();
    while (c != '"') {
        result += c;
        c = in.take();
    }

    in.take(); //skip "

    return result;
}

void JsonReader::parseTrue(JsonInputStream& in, JsonValue& value) const {
    assert(in.peek() == 't');
    in.take(); //skip t

    if (consume(in, 'r') && consume(in, 'u') && consume(in, 'e')) {
        value.setBool(true);
    } else {
        assert(false);
    }
}

void JsonReader::parseFalse(JsonInputStream& in, JsonValue& value) const {
    assert(in.peek() == 'f');
    in.take(); //skip f

    if (consume(in, 'a') && consume(in, 'l') && consume(in, 's') && consume(in, 'e')) {
        value.setBool(false);
    } else {
        assert(false);
    }
}

void JsonReader::parseArray(JsonInputStream& in, JsonValue& value) const {
    assert(in.peek() == '[');
    in.take(); //skip [
    skipSpaceAndComments(in);

    //valueを配列に設定する
    value.setArray();
    auto& a = value.a;

    //空配列かチェック
    if (consume(in, ']')) {
        return;
    }

    while (true) {
        //配列の末尾に要素を追加していく
        parseValue(in, a.emplace_back());

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

void JsonReader::skipSpaceAndComments(JsonInputStream& in) const {
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

void JsonReader::skipSpace(JsonInputStream& in) const {
    //スペース、改行、リターン、タブをスキップする
    char c = in.peek();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        c = in.take();
    }
}

bool JsonReader::consume(JsonInputStream& in, char expect) const {
    if (in.peek() == expect) {
        in.take();
        return true;
    } else {
        return false;
    }
}
