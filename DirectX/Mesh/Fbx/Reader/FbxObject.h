#pragma once

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

struct FbxProperties {
    std::string type;
    std::string type2;
    std::string unknown;
    std::string value;
};

struct FbxConnections {
    std::string related;
    unsigned child = 0;
    unsigned parent = 0;
    std::string value;
};

using FbxValueArray = std::vector<std::string>;
using FbxProperties70 = std::unordered_map<std::string, FbxProperties>;

struct FbxObject {
    //オブジェクトの属性
    std::vector<std::string> attributes;
    //key: 要素名、value: 文字列要素
    std::unordered_map<std::string, std::string> values;
    //key: 要素名、value: 配列文字列要素
    std::unordered_map<std::string, FbxValueArray> valueArray;
    //子オブジェクト
    std::unordered_multimap<std::string, FbxObject> children;
    //プロパティ
    FbxProperties70 properties;
    //コネクション
    std::vector<FbxConnections> connections;



    //子オブジェクトを取得する
    const FbxObject& getObject(const std::string& name) const {
        return children.find(name)->second;
    }

    //nameとattributeに一致する子オブジェクトを取得する
    const FbxObject& getObject(const std::string& name, const std::string& attribute) const {
        //まずnameと一致する子オブジェクトを取得する
        auto range = children.equal_range(name);
        for (auto& r = range.first; r != range.second; ++r) {
            //次にattributeと一致するか確かめる
            const auto& attr = r->second.attributes;
            for (const auto& a : attr) {
                if (a == attribute) {
                    return r->second;
                }
            }
        }

        assert(false);
    }

    //nameと一致するプロパティが存在するか
    bool hasObject(const std::string& name) const {
        return (children.find(name) != children.end());
    }

    //keyに対応する値を取得する
    const std::string& getValue(const std::string& key) const {
        return values.at(key);
    }

    //keyに対応する配列を取得する
    const FbxValueArray& getArray(const std::string& key) const {
        return valueArray.at(key);
    }

    //keyと一致する配列が存在するか
    bool hasArray(const std::string& key) const {
        return (valueArray.find(key) != valueArray.end());
    }

    //nameと一致するプロパティを取得する
    const FbxProperties& getProperties(const std::string& name) const {
        return properties.at(name);
    }

    //nameと一致するプロパティが存在するか
    bool hasProperties(const std::string& name) const {
        return (properties.find(name) != properties.end());
    }

    //オブジェクトのノードIDを取得する
    //アトリビュートの1番目が数値である必要あり
    unsigned getNodeId() const {
        auto id = std::stoi(attributes[0]);
        return static_cast<unsigned>(id);
    }
};
