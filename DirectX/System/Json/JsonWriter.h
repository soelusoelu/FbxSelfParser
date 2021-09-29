#pragma once

#include "JsonObject.h"
#include <fstream>
#include <string>

class JsonWriter {
public:
    JsonWriter();
    ~JsonWriter();
    JsonWriter(const JsonWriter&) = delete;
    JsonWriter& operator=(const JsonWriter&) = delete;

    void write(const char* filePath, const JsonObject& inObject) const;

private:
    void writeObject(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const;
    void writeValues(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const;
    void writeValueArray(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const;
    void writeObjectsArray(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const;

    void writeTab(std::ofstream& out, int count) const;
    void writeNewLine(std::ofstream& out) const;
    void writeColon(std::ofstream& out) const;

    void writeString(std::ofstream& out, const std::string& in) const;
    void writeNumber(std::ofstream& out, const std::string& in) const;
};
