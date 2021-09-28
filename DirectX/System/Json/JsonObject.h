#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct JsonObject;
using JsonObjectPtr = std::shared_ptr<JsonObject>;
using JsonObjectArray = std::vector<JsonObjectPtr>;
using JsonValueArray = std::vector<std::string>;

struct JsonObject {
    //key: 要素名、value: 文字列要素
    std::unordered_map<std::string, std::string> values;
    //key: 要素名、value: 配列文字列要素
    std::unordered_map<std::string, JsonValueArray> valueArray;
    //子オブジェクト
    std::unordered_map<std::string, JsonObjectPtr> children;
    //配列子オブジェクト
    std::unordered_map<std::string, JsonObjectArray> arrayChildren;

    //子オブジェクトを取得する
    JsonObject& getObject(const std::string& name) const {
        return *getObjectPtr(name);
    }

    //子オブジェクトを取得する
    const JsonObjectPtr& getObjectPtr(const std::string& name) const {
        return children.find(name)->second;
    }

    //keyと一致する子オブジェクトが存在するか
    bool hasObject(const std::string& key) const {
        return (children.find(key) != children.end());
    }

    //配列子オブジェクトを取得する
    const JsonObjectArray& getObjects(const std::string& name) const {
        return arrayChildren.find(name)->second;
    }

    //keyと一致する配列子オブジェクトが存在するか
    bool hasObjects(const std::string& key) const {
        return (arrayChildren.find(key) != arrayChildren.end());
    }

    //keyに対応する値を取得する
    const std::string& getValue(const std::string& key) const {
        return values.at(key);
    }

    //keyと一致する値が存在するか
    bool hasValue(const std::string& key) const {
        return (values.find(key) != values.end());
    }

    //keyに対応する配列を取得する
    const JsonValueArray& getArray(const std::string& key) const {
        return valueArray.at(key);
    }

    //keyと一致する配列が存在するか
    bool hasArray(const std::string& key) const {
        return (valueArray.find(key) != valueArray.end());
    }
};
