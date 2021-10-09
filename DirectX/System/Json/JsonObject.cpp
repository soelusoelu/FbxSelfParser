#include "JsonObject.h"
#include "JsonValue.h"

void JsonObject::setValue(const std::string& key, const std::shared_ptr<JsonValue>& value) {
    auto itr = values.find(key);
    if (itr != values.end()) {
        itr->second = value;
    } else {
        values.emplace(key, value);
        keys.emplace_back(key);
    }
}

void JsonObject::setValue(const std::string& key, int value) {
    auto v = std::make_shared<JsonValue>();
    v->setInt(value);
    values.emplace(key, v);
    keys.emplace_back(key);
}

void JsonObject::setValue(const std::string& key, float value) {
    auto v = std::make_shared<JsonValue>();
    v->setFloat(value);
    values.emplace(key, v);
    keys.emplace_back(key);
}

void JsonObject::setValue(const std::string& key, const std::string& value) {
    auto v = std::make_shared<JsonValue>();
    v->setString(value);
    values.emplace(key, v);
    keys.emplace_back(key);
}

void JsonObject::setValue(const std::string& key, bool value) {
    auto v = std::make_shared<JsonValue>();
    v->setBool(value);
    values.emplace(key, v);
    keys.emplace_back(key);
}

JsonValue& JsonObject::getValue(const std::string& key) const {
    return *values.at(key);
}

bool JsonObject::hasValue(const std::string& key) const {
    return (values.find(key) != values.end());
}
