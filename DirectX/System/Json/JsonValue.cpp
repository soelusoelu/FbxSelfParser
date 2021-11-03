#include "JsonValue.h"
#include "JsonObject.h"
#include <cassert>

JsonValue::JsonValue()
    : b(true)
    , o(nullptr)
    , f(JsonValueFlag::NONE)
{
}

JsonValue::JsonValue(JsonValueFlag flag)
    : b(true)
    , o(nullptr)
    , f(flag)
{
    if (flag == JsonValueFlag::OBJECT) {
        o = std::make_shared<JsonObject>();
    }
}

JsonValue::JsonValue(const JsonValue& rhs)
    : b(true)
    , o(nullptr)
    , f(rhs.f)
{
    //余分なコピーを防ぐため
    if (f == JsonValueFlag::BOOL) {
        b = rhs.b;
    } else if (f == JsonValueFlag::INT) {
        n.i = rhs.n.i;
    } else if (f == JsonValueFlag::FLOAT) {
        n.f = rhs.n.f;
    } else if (f == JsonValueFlag::STRING) {
        s = rhs.s;
    } else if (f == JsonValueFlag::ARRAY) {
        a = rhs.a;
    } else if (f == JsonValueFlag::OBJECT) {
        o = rhs.o;
    } else {
        assert(false);
    }
}

JsonValue& JsonValue::operator=(const JsonValue& rhs) {
    //余分なコピーを防ぐため
    auto flag = rhs.f;
    if (flag == JsonValueFlag::BOOL) {
        b = rhs.b;
    } else if (flag == JsonValueFlag::INT) {
        n.i = rhs.n.i;
    } else if (flag == JsonValueFlag::FLOAT) {
        n.f = rhs.n.f;
    } else if (flag == JsonValueFlag::STRING) {
        s = rhs.s;
    } else if (flag == JsonValueFlag::ARRAY) {
        a = rhs.a;
    } else if (flag == JsonValueFlag::OBJECT) {
        o = rhs.o;
    } else {
        assert(false);
    }

    f = flag;

    return *this;
}

bool JsonValue::getBool() const {
    return b;
}

int JsonValue::getInt() const {
    return n.i;
}

float JsonValue::getFloat() const {
    return n.f;
}

const std::string& JsonValue::getString() const {
    return s;
}

const std::vector<JsonValue>& JsonValue::getArray() const {
    return a;
}

JsonObject& JsonValue::getObject() const {
    return *o;
}

void JsonValue::setBool(bool value) {
    assert(f == JsonValueFlag::NONE || f == JsonValueFlag::BOOL);
    b = value;
    f = JsonValueFlag::BOOL;
}

void JsonValue::setInt(int value) {
    assert(f == JsonValueFlag::NONE || f == JsonValueFlag::INT);
    n.i = value;
    f = JsonValueFlag::INT;
}

void JsonValue::setFloat(float value) {
    assert(f == JsonValueFlag::NONE || f == JsonValueFlag::FLOAT);
    n.f = value;
    f = JsonValueFlag::FLOAT;
}

void JsonValue::setString(const std::string& value) {
    assert(f == JsonValueFlag::NONE || f == JsonValueFlag::STRING);
    s = value;
    f = JsonValueFlag::STRING;
}

const std::vector<JsonValue>& JsonValue::setArray() {
    f = JsonValueFlag::ARRAY;

    return a;
}

JsonObject& JsonValue::setObject() {
    if (!o) {
        o = std::make_shared<JsonObject>();
    }
    f = JsonValueFlag::OBJECT;

    return *o;
}

void JsonValue::pushBack(bool value) {
    assert(f == JsonValueFlag::ARRAY);
    auto& v = a.emplace_back();
    v.setBool(value);
}

void JsonValue::pushBack(int value) {
    auto& v = a.emplace_back();
    v.setInt(value);
}

void JsonValue::pushBack(float value) {
    auto& v = a.emplace_back();
    v.setFloat(value);
}

void JsonValue::pushBack(const std::string& value) {
    auto& v = a.emplace_back();
    v.setString(value);
}

bool JsonValue::isBool() const {
    return (f == JsonValueFlag::BOOL);
}

bool JsonValue::isInt() const {
    return (f == JsonValueFlag::INT);
}

bool JsonValue::isFloat() const {
    return (f == JsonValueFlag::FLOAT);
}

bool JsonValue::isString() const {
    return (f == JsonValueFlag::STRING);
}

bool JsonValue::isArray() const {
    return (f == JsonValueFlag::ARRAY);
}

bool JsonValue::isObject() const {
    return (f == JsonValueFlag::OBJECT);
}
