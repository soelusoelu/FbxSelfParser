#pragma once

#include "../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <string>

class FileOperator {
public:
    FileOperator(const std::string& className);
    ~FileOperator();
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {};
    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {};

    //ファイルの保存と読み込み
    void writeAndRead(
        rapidjson::Value& inObj,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );

private:
    FileOperator(const FileOperator&) = delete;
    FileOperator& operator=(const FileOperator&) = delete;

private:
    std::string mClassName;
};
