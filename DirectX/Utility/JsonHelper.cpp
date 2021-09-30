#include "JsonHelper.h"
#include "../System/Json/JsonValue.h"
#include <cassert>

void JsonHelper::getSet(
    int& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setInt(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getInt(value, name, inObject);
    }
}

void JsonHelper::getSet(
    float& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setFloat(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getFloat(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::string& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setString(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getString(value, name, inObject);
    }
}

void JsonHelper::getSet(
    bool& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
)
{
    if (mode == FileMode::SAVE) {
        setBool(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getBool(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Vector2& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector2(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getVector2(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Vector3& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector3(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getVector3(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Vector4& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector4(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getVector4(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Quaternion& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setQuaternion(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getQuaternion(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::vector<int>& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setIntArray(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getIntArray(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::vector<std::string>& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setStringArray(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getStringArray(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::vector<Vector3>& value,
    const std::string& name,
    JsonObject& inObject,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector3Array(value, name, inObject);
    } else if (mode == FileMode::LOAD) {
        getVector3Array(value, name, inObject);
    }
}

bool JsonHelper::getInt(
    int& out,
    const std::string& name,
    const JsonObject& inObject
) {
    //プロパティが存在するか
    if (!inObject.hasValue(name)) {
        return false;
    }

    //値を取得する
    const auto& value = inObject.getValue(name);

    assert(value.isInt());

    //プロパティの取得に成功
    out = value.getInt();

    return true;
}

bool JsonHelper::getFloat(
    float& out,
    const std::string& name,
    const JsonObject& inObject
) {
    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isFloat());

    out = value.getFloat();

    return true;
}

bool JsonHelper::getString(
    std::string& out,
    const std::string& name,
    const JsonObject& inObject
) {
    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isString());

    out = value.getString();

    return true;
}

bool JsonHelper::getBool(
    bool& out,
    const std::string& name,
    const JsonObject& inObject
) {
    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isBool());

    out = value.getBool();

    return true;
}

bool JsonHelper::getVector2(
    Vector2& out,
    const std::string& name,
    const JsonObject& inObject
) {
    constexpr int VECTOR2_VALUE_COUNT = 2;

    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isArray());
    const auto& a = value.getArray();

    assert(a.size() == VECTOR2_VALUE_COUNT);
    assert(a[0].isFloat() && a[1].isFloat());

    out.x = a[0].getFloat();
    out.y = a[1].getFloat();

    return true;
}

bool JsonHelper::getVector3(
    Vector3& out,
    const std::string& name,
    const JsonObject& inObject
) {
    constexpr int VECTOR3_VALUE_COUNT = 3;

    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isArray());
    const auto& a = value.getArray();

    assert(a.size() == VECTOR3_VALUE_COUNT);
    assert(a[0].isFloat() && a[1].isFloat() && a[2].isFloat());

    out.x = a[0].getFloat();
    out.y = a[1].getFloat();
    out.z = a[2].getFloat();

    return true;
}

bool JsonHelper::getVector4(
    Vector4& out,
    const std::string& name,
    const JsonObject& inObject
) {
    constexpr int VECTOR4_VALUE_COUNT = 4;

    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isArray());
    const auto& a = value.getArray();

    assert(a.size() == VECTOR4_VALUE_COUNT);
    assert(a[0].isFloat() && a[1].isFloat() && a[2].isFloat() && a[3].isFloat());

    out.x = a[0].getFloat();
    out.y = a[1].getFloat();
    out.z = a[2].getFloat();
    out.w = a[3].getFloat();

    return true;
}

bool JsonHelper::getQuaternion(
    Quaternion& out,
    const std::string& name,
    const JsonObject& inObject
) {
    Vector4 temp;
    if (!getVector4(temp, name, inObject)) {
        return false;
    }

    static_assert(sizeof(Vector4) == sizeof(Quaternion), "Vector4 and Quaternion are not the same size.");
    memcpy(&out, &temp, sizeof(Quaternion));

    return true;
}

bool JsonHelper::getIntArray(
    std::vector<int>& out,
    const std::string& name,
    const JsonObject& inObject
) {
    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isArray());
    const auto& a = value.getArray();

    if (a.empty()) {
        return false;
    }

#ifdef _DEBUG
    for (const auto& v : a) {
        assert(v.isInt());
    }
#endif // _DEBUG

    auto size = a.size();
    out.resize(size);
    for (size_t i = 0; i < size; ++i) {
        out[i] = a[i].getInt();
    }

    return true;
}

bool JsonHelper::getStringArray(
    std::vector<std::string>& out,
    const std::string& name,
    const JsonObject& inObject
) {
    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isArray());
    const auto& a = value.getArray();

    if (a.empty()) {
        return false;
    }

#ifdef _DEBUG
    for (const auto& v : a) {
        assert(v.isString());
    }
#endif // _DEBUG

    auto size = a.size();
    out.resize(size);
    for (size_t i = 0; i < size; ++i) {
        out[i] = a[i].getString();
    }

    return true;
}

bool JsonHelper::getVector3Array(
    std::vector<Vector3>& out,
    const std::string& name,
    const JsonObject& inObject
) {
    constexpr int VECTOR3_VALUE_COUNT = 3;

    if (!inObject.hasValue(name)) {
        return false;
    }

    const auto& value = inObject.getValue(name);

    assert(value.isArray());
    const auto& a = value.getArray();

    if (a.empty()) {
        return false;
    }

#ifdef _DEBUG
    for (const auto& v : a) {
        assert(v.isFloat());
    }
#endif // _DEBUG

    auto size = a.size();
    assert(size % VECTOR3_VALUE_COUNT == 0);
    auto sizeDiv3 = size / VECTOR3_VALUE_COUNT;
    out.resize(sizeDiv3);

    for (size_t i = 0; i < sizeDiv3; ++i) {
        auto idx = i * VECTOR3_VALUE_COUNT;
        out[i].x = a[idx].getFloat();
        out[i].y = a[idx + 1].getFloat();
        out[i].z = a[idx + 2].getFloat();
    }

    return true;
}

void JsonHelper::setInt(
    int value,
    const std::string& name,
    JsonObject& inObject
) {
    //すでに値を保持しているなら上書き、ないなら追加
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isInt());
        v.n.i = value;
    } else {
        inObject.setValue(name, value);
    }
}

void JsonHelper::setFloat(
    float value,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isFloat());
        v.n.f = value;
    } else {
        inObject.setValue(name, value);
    }
}

void JsonHelper::setString(
    const std::string& value,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isString());
        v.s = value;
    } else {
        inObject.setValue(name, value);
    }
}

void JsonHelper::setBool(
    bool value,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isBool());
        v.b = value;
    } else {
        inObject.setValue(name, value);
    }
}

void JsonHelper::setVector2(
    const Vector2& value,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isArray());
        auto& a = v.a;
        assert(a.size() == 2);
        assert(a[0].isFloat() && a[1].isFloat());
        a[0].n.f = value.x;
        a[1].n.f = value.y;
    } else {
        auto v = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
        v->pushBack(value.x);
        v->pushBack(value.y);
        inObject.setValue(name, v);
    }
}

void JsonHelper::setVector3(
    const Vector3& value,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isArray());
        auto& a = v.a;
        assert(a.size() == 3);
        assert(a[0].isFloat() && a[1].isFloat() && a[2].isFloat());
        a[0].n.f = value.x;
        a[1].n.f = value.y;
        a[2].n.f = value.z;
    } else {
        auto v = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
        v->pushBack(value.x);
        v->pushBack(value.y);
        v->pushBack(value.z);
        inObject.setValue(name, v);
    }
}

void JsonHelper::setVector4(
    const Vector4& value,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isArray());
        auto& a = v.a;
        assert(a.size() == 4);
        assert(a[0].isFloat() && a[1].isFloat() && a[2].isFloat() && a[3].isFloat());
        a[0].n.f = value.x;
        a[1].n.f = value.y;
        a[2].n.f = value.z;
        a[3].n.f = value.w;
    } else {
        auto v = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
        v->pushBack(value.x);
        v->pushBack(value.y);
        v->pushBack(value.z);
        v->pushBack(value.w);
        inObject.setValue(name, v);
    }
}

void JsonHelper::setQuaternion(
    const Quaternion& value,
    const std::string& name,
    JsonObject& inObject
) {
    Vector4 tmp(value.x, value.y, value.z, value.w);
    setVector4(tmp, name, inObject);
}

void JsonHelper::setIntArray(
    const std::vector<int>& values,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isArray());
        auto& a = v.a;
        auto size = values.size();
        a.resize(size);
        for (size_t i = 0; i < size; ++i) {
            a[i].setInt(values[i]);
        }
    } else {
        auto v = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
        for (const auto& x : values) {
            v->pushBack(x);
        }
        inObject.setValue(name, v);
    }
}

void JsonHelper::setStringArray(
    const std::vector<std::string>& values,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isArray());
        auto& a = v.a;
        auto size = values.size();
        a.resize(size);
        for (size_t i = 0; i < size; ++i) {
            a[i].setString(values[i]);
        }
    } else {
        auto v = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
        for (const auto& x : values) {
            v->pushBack(x);
        }
        inObject.setValue(name, v);
    }
}

void JsonHelper::setVector3Array(
    const std::vector<Vector3>& values,
    const std::string& name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        auto& v = inObject.getValue(name);
        assert(v.isArray());
        auto& a = v.a;
        auto size = values.size();
        a.resize(size * 3);
        for (size_t i = 0; i < size; ++i) {
            const auto& vec3 = values[i];
            auto idx = i * 3;
            a[idx].setFloat(vec3.x);
            a[idx + 1].setFloat(vec3.y);
            a[idx + 2].setFloat(vec3.z);
        }
    } else {
        auto v = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
        for (const auto& x : values) {
            v->pushBack(x.x);
            v->pushBack(x.y);
            v->pushBack(x.z);
        }
        inObject.setValue(name, v);
    }
}
