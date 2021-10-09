#pragma once

#include <string>

struct JsonObject;
struct JsonValue;
class JsonInputStream;

class JsonReader {
public:
    JsonReader();
    ~JsonReader();
    JsonReader(const JsonReader&) = delete;
    JsonReader& operator=(const JsonReader&) = delete;

    //JsonInputStreamの中身を解析する
    void parse(JsonInputStream& in, JsonObject& value) const;

private:
    //値を解析する
    void parseValue(JsonInputStream& in, JsonValue& value) const;
    //{}で囲まれたオブジェクトを解析する
    void parseObject(JsonInputStream& in, JsonObject& value) const;
    //数値を解析する
    void parseNumber(JsonInputStream& in, JsonValue& value) const;
    //""で囲まれた文字列を解析する
    void parseString(JsonInputStream& in, JsonValue& value) const;
    std::string parseString(JsonInputStream& in) const;
    //ブーリアンを解析する
    void parseTrue(JsonInputStream& in, JsonValue& value) const;
    void parseFalse(JsonInputStream& in, JsonValue& value) const;
    //配列を解析する
    void parseArray(JsonInputStream& in, JsonValue& value) const;

    //スペースとコメントをスキップする
    void skipSpaceAndComments(JsonInputStream& in) const;
    //スペースをスキップする
    void skipSpace(JsonInputStream& in) const;
    //expectが現在の読み取り位置の文字と一致していたら、読み取り位置を一つ進める
    bool consume(JsonInputStream& in, char expect) const;
};
