#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using JsonValueArray = std::vector<std::string>;

struct JsonObject {
    //key: 要素名、value: 文字列要素
    std::unordered_map<std::string, std::string> values;
    //key: 要素名、value: 配列文字列要素
    std::unordered_map<std::string, JsonValueArray> valueArray;
    //子オブジェクト
    std::unordered_map<std::string, std::unique_ptr<JsonObject>> children;
};
