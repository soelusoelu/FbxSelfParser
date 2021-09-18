#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

struct FbxProperties {
    std::string name;
    std::string type;
    std::string type2;
    std::string unknown;
    std::string value;
};

using FbxValueArray = std::vector<std::string>;
using FbxProperties70 = std::vector<FbxProperties>;

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
    FbxProperties70 properties;



    //子オブジェクトを取得する
    const FbxObject& getObject(const std::string& name) const {
        for (const auto& c : children) {
            if (c.name == name) {
                return c;
            }
        }

        assert(false);
    }

    //nameとattributeに一致する子オブジェクトを取得する
    const FbxObject& getObject(const std::string& name, const std::string& attribute) const {
        for (const auto& c : children) {
            //まずnameと一致するか確かめる
            if (c.name != name) {
                continue;
            }

            //次にattributeと一致するか確かめる
            for (const auto& a : c.attributes) {
                if (a == attribute) {
                    return c;
                }
            }
        }

        assert(false);
    }

    //nameと一致するプロパティが存在するか
    bool hasObject(const std::string& name) const {
        for (const auto& c : children) {
            if (c.name == name) {
                return true;
            }
        }

        return false;
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
    const FbxProperties& getProperties(const std::string& name) const {
        for (const auto& p : properties) {
            if (p.name == name) {
                return p;
            }
        }

        assert(false);
    }

    //nameと一致するプロパティが存在するか
    bool hasProperties(const std::string& name) const {
        for (const auto& p : properties) {
            if (p.name == name) {
                return true;
            }
        }

        return false;
    }
};
