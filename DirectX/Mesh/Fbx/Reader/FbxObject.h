#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

using FbxValueArray = std::vector<std::string>;

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
    std::unordered_map<std::string, FbxValueArray> valueArray;
    //子オブジェクト
    std::vector<FbxObject> children;
    //プロパティ
    std::vector<FbxProperties70> properties;



    //子オブジェクトを取得する
    const FbxObject& getObject(const std::string& name) const {
        for (const auto& c : children) {
            if (c.name == name) {
                return c;
            }
        }

        assert(false);
    }

    //keyに対応する値を取得する
    const std::string& getValue(const std::string& key) const {
        return values.at(key);
    }

    //keyに対応する配列を取得する
    const FbxValueArray& getArray(const std::string& key) const {
        return valueArray.at(key);
    }

    //nameと一致するプロパティを取得する
    const FbxProperties70& getProperties(const std::string& name) const {
        for (const auto& p : properties) {
            if (p.name == name) {
                return p;
            }
        }

        assert(false);
    }
};
