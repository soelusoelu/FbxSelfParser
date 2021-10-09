#include "JsonWriter.h"
#include "JsonObject.h"
#include "JsonOutputStream.h"
#include "JsonValue.h"
#include "../../Math/Math.h"
#include <cassert>
#include <fstream>

JsonWriter::JsonWriter() = default;

JsonWriter::~JsonWriter() = default;

void JsonWriter::write(const char* filePath, const JsonObject& inObject) const {
    JsonOutputStream stream;

    writeObjectStart(stream);
    writeNewLine(stream);

    int tabCount = 0;
    writeValues(stream, inObject, tabCount);

    writeObjectEnd(stream);

    //文字列の末尾にヌル文字を追加して文字列化は終わり
    stream.addSize(1);
    stream.push('\0');

    //ファイルに書き込む
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        return;
    }

    outFile << stream.getStringBufer();
}

void JsonWriter::writeValues(JsonOutputStream& out, const JsonObject& inObject, int& tabCount) const {
    ++tabCount;

    const auto& values = inObject.values;
    const auto& keys = inObject.keys;
    assert(values.size() == keys.size());
    int num = values.size();
    //オブジェクトが保有している全要素を出力していく
    for (const auto& key : keys) {
        writeTab(out, tabCount);

        //名前がない要素もある(オブジェクト配列など)ため、名前がある場合のみ出力
        if (key.length() > 0) {
            writeString(out, key);

            writeColon(out);
        }

        //要素を出力する
        const auto& v = *values.at(key);
        writeValue(out, v, tabCount);

        --num;
        if (num > 0) {
            writeComma(out);
        }

        writeNewLine(out);
    }

    --tabCount;
}

void JsonWriter::writeValue(JsonOutputStream& out, const JsonValue& inValue, int& tabCount) const {
    if (inValue.isBool()) {
        writeBool(out, inValue.getBool());
    } else if (inValue.isInt()) {
        writeInt(out, inValue.getInt());
    } else if (inValue.isFloat()) {
        writeFloat(out, inValue.getFloat());
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

void JsonWriter::writeObject(JsonOutputStream& out, const JsonObject& inObject, int& tabCount) const {
    writeObjectStart(out);
    //空オブジェクトなら早期リターン
    if (inObject.values.empty()) {
        writeObjectEnd(out);
        return;
    }

    writeNewLine(out);

    //オブジェクトの中身の出力はwriteValuesにまかせる
    writeValues(out, inObject, tabCount);

    writeTab(out, tabCount);
    writeObjectEnd(out);
}

void JsonWriter::writeArray(JsonOutputStream& out, const std::vector<JsonValue>& inValues, int& tabCount) const {
    writeArrayStart(out);
    //空配列なら早期リターン
    if (inValues.empty()) {
        writeArrayEnd(out);
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
            writeComma(out);
        }

        writeNewLine(out);
    }

    --tabCount;

    writeTab(out, tabCount);
    writeArrayEnd(out);
}

void JsonWriter::writeInt(JsonOutputStream& out, int in) const {
    auto s = std::to_string(in);
    out.addSize(s.length());
    for (const auto& c : s) {
        out.push(c);
    }
}

void JsonWriter::writeFloat(JsonOutputStream& out, float in) const {
    //値が0なら0.0を入力して終わり
    if (Math::nearZero(in)) {
        out.addSize(3);
        out.push('0');
        out.push('.');
        out.push('0');
        return;
    }

    //数値を文字列化
    auto s = std::to_string(in);

    //文字列にした際の無駄な0を削除する
    while (s.back() == '0') {
        s.pop_back();
    }
    //小数部分がすべて0だったとき最後に0を足す
    if (s.back() == '.') {
        s += '0';
    }

    out.addSize(s.length());
    for (const auto& c : s) {
        out.push(c);
    }
}

void JsonWriter::writeBool(JsonOutputStream& out, bool in) const {
    if (in) {
        out.addSize(4);
        out.push('t');
        out.push('r');
        out.push('u');
        out.push('e');
    } else {
        out.addSize(5);
        out.push('f');
        out.push('a');
        out.push('l');
        out.push('s');
        out.push('e');
    }
}

void JsonWriter::writeString(JsonOutputStream& out, const std::string& in) const {
    //文字列数 + ダブルクォーテーション2つ分
    out.addSize(in.length() + 2);

    out.push('"');

    for (auto itr = in.begin(); itr < in.end(); ++itr) {
        char c = *itr;

        if (c == '\\') {
            //ファイルパス文字列の正規表現操作
            out.push(c);

            ++itr;
            char next = *itr;
            if (next != '\\') {
                out.addSize(1);
                out.push('\\');
            }

            out.push(next);
        } else {
            out.push(c);
        }
    }

    out.push('"');
}

void JsonWriter::writeObjectStart(JsonOutputStream& out) const {
    out.addSize(1);
    out.push('{');
}

void JsonWriter::writeObjectEnd(JsonOutputStream& out) const {
    out.addSize(1);
    out.push('}');
}

void JsonWriter::writeArrayStart(JsonOutputStream& out) const {
    out.addSize(1);
    out.push('[');
}

void JsonWriter::writeArrayEnd(JsonOutputStream& out) const {
    out.addSize(1);
    out.push(']');
}

void JsonWriter::writeComma(JsonOutputStream& out) const {
    out.addSize(1);
    out.push(',');
}

void JsonWriter::writeTab(JsonOutputStream& out, int count) const {
    out.addSize(count);

    for (int i = 0; i < count; ++i) {
        out.push('\t');
    }
}

void JsonWriter::writeNewLine(JsonOutputStream& out) const {
    out.addSize(1);
    out.push('\n');
}

void JsonWriter::writeColon(JsonOutputStream& out) const {
    out.addSize(2);
    out.push(':');
    out.push(' ');
}
