#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct FbxValue {
    std::string name;
    std::vector<std::string> attributes;

    //key: 要素名、value: 文字列要素
    std::unordered_map<std::string, std::string> values;
    std::vector<FbxValue> children;
};
