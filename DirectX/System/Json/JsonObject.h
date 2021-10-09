#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct JsonValue;

struct JsonObject {
    //key: 要素名, value: JsonValue要素
    std::unordered_map<std::string, std::shared_ptr<JsonValue>> values;
    //要素名追加順
    std::vector<std::string> keys;

    //JsonValueを追加する
    void setValue(const std::string& key, const std::shared_ptr<JsonValue>& value);
    //intを追加する
    void setValue(const std::string& key, int value);
    //floatを追加する
    void setValue(const std::string& key, float value);
    //stringを追加する
    void setValue(const std::string& key, const std::string& value);
    //boolを追加する
    void setValue(const std::string& key, bool value);
    //keyと一致する要素を取得する
    JsonValue& getValue(const std::string& key) const;
    //keyと一致する要素を保持しているか
    bool hasValue(const std::string& key) const;
};
