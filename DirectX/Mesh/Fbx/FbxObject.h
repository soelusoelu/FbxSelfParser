#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using FbxArrayValue = std::vector<std::string>;

struct FbxProperties70 {
    std::string name;
    std::string type;
    std::string type2;
    std::string unknown;
    std::string value;
};

struct FbxObject {
    //オブジェクト名
    std::string name;
    //オブジェクトの属性
    std::vector<std::string> attributes;
    //key: 要素名、value: 文字列要素
    std::unordered_map<std::string, std::string> values;
    //key: 要素名、value: 配列文字列要素
    std::unordered_map<std::string, FbxArrayValue> arrayValues;
    //子オブジェクト
    std::vector<FbxObject> children;
    //プロパティ
    std::vector<FbxProperties70> properties;
};
