#pragma once

#include <string>
#include <vector>
#include <unordered_map>

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
    //子オブジェクト
    std::vector<FbxObject> children;

    std::vector<FbxProperties70> properties;
};
