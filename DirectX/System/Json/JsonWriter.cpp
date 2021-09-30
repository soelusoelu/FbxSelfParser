#include "JsonWriter.h"
#include "JsonObject.h"
#include "JsonValue.h"
#include <cassert>

JsonWriter::JsonWriter() = default;

JsonWriter::~JsonWriter() = default;

void JsonWriter::write(const char* filePath, const JsonObject& inObject) const {
    //ファイルに書き込む
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        return;
    }

    outFile << '{';
    writeNewLine(outFile);

    int tabCount = 0;
    writeValues(outFile, inObject, tabCount);

    outFile << '}';
}

void JsonWriter::writeValues(std::ofstream& out, const JsonObject& inObject, int& tabCount) const {
    ++tabCount;

    const auto& values = inObject.values;
    int num = values.size();
    //オブジェクトが保有している全要素を出力していく
    for (const auto& value : values) {
        writeTab(out, tabCount);

        //名前がない要素もある(オブジェクト配列など)ため、名前がある場合のみ出力
        if (value.first.size() > 0) {
            writeString(out, value.first);

            writeColon(out);
        }

        //要素を出力する
        const auto& v = *value.second;
        writeValue(out, v, tabCount);

        --num;
        if (num > 0) {
            out << ',';
        }

        writeNewLine(out);
    }

    --tabCount;
}

void JsonWriter::writeValue(std::ofstream& out, const JsonValue& inValue, int& tabCount) const {
    if (inValue.isBool()) {
        out << (inValue.getBool() ? true : false);
    } else if (inValue.isInt()) {
        out << inValue.getInt();
    } else if (inValue.isFloat()) {
        out << std::to_string(inValue.getFloat());
    } else if (inValue.isString()) {
        writeString(out, inValue.getString());
    } else if (inValue.isArray()) {
        writeArray(out, inValue.getArray(), tabCount);
    } else if (inValue.isObject()) {
        writeObject(out, inValue.getObject(), tabCount);
    } else {
        assert(false);
    }
}

void JsonWriter::writeObject(std::ofstream& out, const JsonObject& inObject, int& tabCount) const {
    out << '{';
    //空オブジェクトなら早期リターン
    if (inObject.values.empty()) {
        out << '}';
        return;
    }

    writeNewLine(out);

    //オブジェクトの中身の出力はwriteValuesにまかせる
    writeValues(out, inObject, tabCount);

    writeTab(out, tabCount);
    out << '}';
}

void JsonWriter::writeArray(std::ofstream& out, const std::vector<JsonValue>& inValues, int& tabCount) const {
    out << '[';
    //空配列なら早期リターン
    if (inValues.empty()) {
        out << ']';
        return;
    }

    writeNewLine(out);

    ++tabCount;

    int num = inValues.size();
    //配列の全要素を出力していく
    for (const auto& v : inValues) {
        writeTab(out, tabCount);

        writeValue(out, v, tabCount);

        --num;
        if (num > 0) {
            out << ',';
        }

        writeNewLine(out);
    }

    --tabCount;

    writeTab(out, tabCount);
    out << ']';
}

void JsonWriter::writeTab(std::ofstream& out, int count) const {
    for (int i = 0; i < count; ++i) {
        out << '\t';
    }
}

void JsonWriter::writeNewLine(std::ofstream& out) const {
    out << '\n';
}

void JsonWriter::writeColon(std::ofstream& out) const {
    out << ": ";
}

void JsonWriter::writeString(std::ofstream& out, const std::string& in) const {
    out << '"' + in + '"';
}
