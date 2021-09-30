#pragma once

#include <fstream>
#include <string>
#include <vector>

struct JsonObject;
struct JsonValue;

class JsonWriter {
public:
    JsonWriter();
    ~JsonWriter();
    JsonWriter(const JsonWriter&) = delete;
    JsonWriter& operator=(const JsonWriter&) = delete;

    //Jsonファイルを出力する
    void write(const char* filePath, const JsonObject& inObject) const;

private:
    //JsonObjectが保有している全要素を出力する
    void writeValues(std::ofstream& out, const JsonObject& inObject, int& tabCount) const;
    //JsonValueが保有している型に応じて要素を出力する
    void writeValue(std::ofstream& out, const JsonValue& inValue, int& tabCount) const;
    //JsonObjectを出力する
    void writeObject(std::ofstream& out, const JsonObject& inObject, int& tabCount) const;
    //JsonValue配列を出力する
    void writeArray(std::ofstream& out, const std::vector<JsonValue>& inValues, int& tabCount) const;

    //指定した数分タブを出力する
    void writeTab(std::ofstream& out, int count) const;
    //改行コードを出力する
    void writeNewLine(std::ofstream& out) const;
    //コロン + スペースを出力する
    void writeColon(std::ofstream& out) const;

    //文字列を出力する
    void writeString(std::ofstream& out, const std::string& in) const;
};
