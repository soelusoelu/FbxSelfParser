#pragma once

#include <string>

struct JsonObject;
struct JsonValue;
class JsonStream;

class JsonReader {
public:
    JsonReader();
    ~JsonReader();
    JsonReader(const JsonReader&) = delete;
    JsonReader& operator=(const JsonReader&) = delete;

    //JsonStreamの中身を解析する
    void parse(JsonStream& in, JsonObject& value) const;

private:
    //値を解析する
    void parseValue(JsonStream& in, JsonValue& value) const;
    //{}で囲まれたオブジェクトを解析する
    void parseObject(JsonStream& in, JsonObject& value) const;
    //数値を解析する
    void parseNumber(JsonStream& in, JsonValue& value) const;
    //""で囲まれた文字列を解析する
    void parseString(JsonStream& in, JsonValue& value) const;
    std::string parseString(JsonStream& in) const;
    //ブーリアンを解析する
    void parseTrue(JsonStream& in, JsonValue& value) const;
    void parseFalse(JsonStream& in, JsonValue& value) const;
    //配列を解析する
    void parseArray(JsonStream& in, JsonValue& value) const;

    //スペースとコメントをスキップする
    void skipSpaceAndComments(JsonStream& in) const;
    //スペースをスキップする
    void skipSpace(JsonStream& in) const;
    //expectが現在の読み取り位置の文字と一致していたら、読み取り位置を一つ進める
    bool consume(JsonStream& in, char expect) const;
};
