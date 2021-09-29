#include "JsonWriter.h"

JsonWriter::JsonWriter() {
}

JsonWriter::~JsonWriter() = default;

void JsonWriter::write(const char* filePath, const JsonObject& inObject) const {
    //ファイルに書き込む
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        return;
    }

    outFile << "{";
    writeNewLine(outFile);

    int tabCount = 0;
    int valuesSize = inObject.values.size();
    int valueArraySize = inObject.valueArray.size();
    int childrenSize = inObject.children.size();
    int arrayChildrenSize = inObject.arrayChildren.size();
    int numAllValuesCount = (valuesSize + valueArraySize + childrenSize + arrayChildrenSize);
    if (valuesSize > 0) {
        writeValues(outFile, inObject, numAllValuesCount, tabCount);
    }
    if (valueArraySize > 0) {
        writeValueArray(outFile, inObject, numAllValuesCount, tabCount);
    }
    if (childrenSize > 0) {
        writeObject(outFile, inObject, numAllValuesCount, tabCount);
    }
    if (arrayChildrenSize > 0) {
        writeObjectsArray(outFile, inObject, numAllValuesCount, tabCount);
    }

    outFile << "}";
}

void JsonWriter::writeObject(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const {
    ++tabCount;

    const auto& children = inObject.children;
    for (const auto& c : children) {
        writeTab(out, tabCount);

        if (c.first.size() > 0) {
            writeString(out, c.first);

            writeColon(out);
        }

        out << "{";
        writeNewLine(out);

        const auto& obj = c.second;
        int valuesSize = obj->values.size();
        int valueArraySize = obj->valueArray.size();
        int childrenSize = obj->children.size();
        int arrayChildrenSize = obj->arrayChildren.size();
        int numAllValuesCount = (valuesSize + valueArraySize + childrenSize + arrayChildrenSize);
        if (valuesSize > 0) {
            writeValues(out, *obj, numAllValuesCount, tabCount);
        }
        if (valueArraySize > 0) {
            writeValueArray(out, *obj, numAllValuesCount, tabCount);
        }
        if (childrenSize > 0) {
            writeObject(out, *obj, numAllValuesCount, tabCount);
        }
        if (arrayChildrenSize > 0) {
            writeObjectsArray(out, *obj, numAllValuesCount, tabCount);
        }

        writeTab(out, tabCount);

        --remainValuesCount;
        out << ((remainValuesCount == 0) ? "}" : "},");
        writeNewLine(out);
    }

    --tabCount;
}

void JsonWriter::writeValues(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const {
    ++tabCount;

    const auto& values = inObject.values;
    for (const auto& v : values) {
        writeTab(out, tabCount);

        writeString(out, v.first);

        writeColon(out);

        writeString(out, v.second);

        --remainValuesCount;
        if (remainValuesCount != 0) {
            out << ",";
        }
        writeNewLine(out);
    }

    --tabCount;
}

void JsonWriter::writeValueArray(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const {
    ++tabCount;

    const auto& valueArray = inObject.valueArray;
    for (const auto& va : valueArray) {
        writeTab(out, tabCount);

        writeString(out, va.first);

        writeColon(out);

        out << "[";
        writeNewLine(out);

        const auto& values = va.second;
        if (values.size() > 0) {
            ++tabCount;

            auto itr = values.begin();
            while (true) {
                writeTab(out, tabCount);
                writeString(out, *itr);

                ++itr;
                if (itr == values.end()) {
                    writeNewLine(out);
                    break;
                } else {
                    out << ",";
                    writeNewLine(out);
                }
            }

            --tabCount;
        }

        writeTab(out, tabCount);

        --remainValuesCount;
        out << ((remainValuesCount == 0) ? "]" : "],");
        writeNewLine(out);
    }

    --tabCount;
}

void JsonWriter::writeObjectsArray(std::ofstream& out, const JsonObject& inObject, int& remainValuesCount, int& tabCount) const {
    ++tabCount;

    const auto& children = inObject.arrayChildren;
    for (const auto& c : children) {
        writeTab(out, tabCount);

        writeString(out, c.first);

        writeColon(out);

        out << "[";
        writeNewLine(out);

        const auto& values = c.second;
        if (values.size() > 0) {
            ++tabCount;

            int tmp = values.size();
            for (const auto& v : values) {
                //writeObject(out, *v, tmp, tabCount);
                writeTab(out, tabCount);

                out << "{";
                writeNewLine(out);

                int valuesSize = v->values.size();
                int valueArraySize = v->valueArray.size();
                int childrenSize = v->children.size();
                int arrayChildrenSize = v->arrayChildren.size();
                int numAllValuesCount = (valuesSize + valueArraySize + childrenSize + arrayChildrenSize);
                if (valuesSize > 0) {
                    writeValues(out, *v, numAllValuesCount, tabCount);
                }
                if (valueArraySize > 0) {
                    writeValueArray(out, *v, numAllValuesCount, tabCount);
                }
                if (childrenSize > 0) {
                    writeObject(out, *v, numAllValuesCount, tabCount);
                }
                if (arrayChildrenSize > 0) {
                    writeObjectsArray(out, *v, numAllValuesCount, tabCount);
                }

                writeTab(out, tabCount);

                --tmp;
                out << ((tmp == 0) ? "}" : "},");
                writeNewLine(out);
            }

            --tabCount;
        }

        writeTab(out, tabCount);

        --remainValuesCount;
        out << ((remainValuesCount == 0) ? "]" : "],");
        writeNewLine(out);
    }

    --tabCount;
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
    out << "\"";
    out << in;
    out << "\"";
}

void JsonWriter::writeNumber(std::ofstream& out, const std::string& in) const {
    out << in;
}
