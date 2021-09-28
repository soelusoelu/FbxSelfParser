#pragma once

#include "../System/Json/JsonObject.h"
#include "../Utility/FileMode.h"
#include <string>

class FileOperator {
public:
    FileOperator(const std::string& className);
    ~FileOperator();
    FileOperator(const FileOperator&) = delete;
    FileOperator& operator=(const FileOperator&) = delete;

    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) {};
    virtual void childSaveAndLoad(JsonObject& inObj, FileMode mode) {};

    //ファイルの保存と読み込み
    void writeAndRead(JsonObject& inObj, FileMode mode);

private:
    std::string mClassName;
};
