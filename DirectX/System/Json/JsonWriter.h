#pragma once

#include <string>
#include <vector>

struct JsonObject;
struct JsonValue;
class JsonOutputStream;

class JsonWriter {
public:
    JsonWriter();
    ~JsonWriter();
    JsonWriter(const JsonWriter&) = delete;
    JsonWriter& operator=(const JsonWriter&) = delete;

    //Jsonファイルを出力する
    void write(const char* filePath, const JsonObject& inObject) const;

private:
    //JsonObjectが保有している全要素を書き込む
    void writeValues(JsonOutputStream& out, const JsonObject& inObject, int& tabCount) const;
    //JsonValueが保有している型に応じて要素を書き込む
    void writeValue(JsonOutputStream& out, const JsonValue& inValue, int& tabCount) const;
    //JsonObjectを書き込む
    void writeObject(JsonOutputStream& out, const JsonObject& inObject, int& tabCount) const;
    //JsonValue配列を書き込む
    void writeArray(JsonOutputStream& out, const std::vector<JsonValue>& inValues, int& tabCount) const;

    //整数を書き込む
    void writeInt(JsonOutputStream& out, int in) const;
    //小数を書き込む
    void writeFloat(JsonOutputStream& out, float in) const;
    //真偽値を書き込む
    void writeBool(JsonOutputStream& out, bool in) const;
    //文字列を書き込む
    void writeString(JsonOutputStream& out, const std::string& in) const;

    //オブジェクト開始文字を書き込む
    void writeObjectStart(JsonOutputStream& out) const;
    //オブジェクト終了文字を書き込む
    void writeObjectEnd(JsonOutputStream& out) const;
    //配列開始文字を書き込む
    void writeArrayStart(JsonOutputStream& out) const;
    //配列終了文字を書き込む
    void writeArrayEnd(JsonOutputStream& out) const;
    //カンマを書き込む
    void writeComma(JsonOutputStream& out) const;
    //指定した数分タブを書き込む
    void writeTab(JsonOutputStream& out, int count) const;
    //改行コードを書き込む
    void writeNewLine(JsonOutputStream& out) const;
    //コロン + スペースを書き込む
    void writeColon(JsonOutputStream& out) const;
};
