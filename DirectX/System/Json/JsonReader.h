#pragma once

#include "JsonObject.h"
#include <string>

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
    void parseValue(JsonStream& in, JsonObject& value, const std::string& key) const;
    //{}で囲まれたオブジェクトを解析する
    void parseObject(JsonStream& in, JsonObject& value, const std::string& key) const;
    //数値を解析する
    void parseNumber(JsonStream& in, std::string& out) const;
    //""で囲まれた文字列を解析する
    void parseString(JsonStream& in, std::string& out) const;
    //配列を解析する
    void parseArray(JsonStream& in, JsonObject& out, const std::string& key) const;

    //スペースとコメントをスキップする
    void skipSpaceAndComments(JsonStream& in) const;
    //スペースをスキップする
    void skipSpace(JsonStream& in) const;
    //expectが現在の読み取り位置の文字と一致していたら、読み取り位置を一つ進める
    bool consume(JsonStream& in, char expect) const;
};
