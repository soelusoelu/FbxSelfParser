#include "JsonHelper.h"

void JsonHelper::getSet(
    int& value,
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
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
    const char* name,
    const JsonObject& inObject
) {
#ifdef _DEBUG
    //プロパティが存在するか
    if (!inObject.hasValue(name)) {
        return false;
    }
#endif // _DEBUG

    //値を取得する
    const auto& value = inObject.getValue(name);

    //プロパティの取得に成功
    out = std::stoi(value);
    return true;
}

bool JsonHelper::getFloat(
    float& out,
    const char* name,
    const JsonObject& inObject
) {
#ifdef _DEBUG
    if (!inObject.hasValue(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getValue(name);

    out = std::stof(value);
    return true;
}

bool JsonHelper::getString(
    std::string& out,
    const char* name,
    const JsonObject& inObject
) {
#ifdef _DEBUG
    if (!inObject.hasValue(name)) {
        return false;
    }
#endif // _DEBUG

    out = inObject.getValue(name);
    return true;
}

bool JsonHelper::getBool(
    bool& out,
    const char* name,
    const JsonObject& inObject
) {
#ifdef _DEBUG
    if (!inObject.hasValue(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getValue(name);

    out = (value == "true");
    return true;
}

bool JsonHelper::getVector2(
    Vector2& out,
    const char* name,
    const JsonObject& inObject
) {
    constexpr int VECTOR2_VALUE_COUNT = 2;

#ifdef _DEBUG
    if (!inObject.hasArray(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getArray(name);

    if (value.size() != VECTOR2_VALUE_COUNT) {
        return false;
    }

    out.x = std::stof(value[0]);
    out.y = std::stof(value[1]);

    return true;
}

bool JsonHelper::getVector3(
    Vector3& out,
    const char* name,
    const JsonObject& inObject
) {
    constexpr int VECTOR3_VALUE_COUNT = 3;

#ifdef _DEBUG
    if (!inObject.hasArray(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getArray(name);

    if (value.size() != VECTOR3_VALUE_COUNT) {
        return false;
    }

    out.x = std::stof(value[0]);
    out.y = std::stof(value[1]);
    out.z = std::stof(value[2]);

    return true;
}

bool JsonHelper::getVector4(
    Vector4& out,
    const char* name,
    const JsonObject& inObject
) {
    constexpr int VECTOR4_VALUE_COUNT = 4;

#ifdef _DEBUG
    if (!inObject.hasArray(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getArray(name);

    if (value.size() != VECTOR4_VALUE_COUNT) {
        return false;
    }

    out.x = std::stof(value[0]);
    out.y = std::stof(value[1]);
    out.z = std::stof(value[2]);
    out.w = std::stof(value[3]);

    return true;
}

bool JsonHelper::getQuaternion(
    Quaternion& out,
    const char* name,
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
    const char* name,
    const JsonObject& inObject
) {
#ifdef _DEBUG
    if (!inObject.hasArray(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getArray(name);

    if (value.empty()) {
        return false;
    }

    auto size = value.size();
    out.resize(size);
    for (size_t i = 0; i < size; ++i) {
        out[i] = std::stoi(value[i]);
    }

    return true;
}

bool JsonHelper::getStringArray(
    std::vector<std::string>& out,
    const char* name,
    const JsonObject& inObject
) {
#ifdef _DEBUG
    if (!inObject.hasArray(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getArray(name);

    if (value.empty()) {
        return false;
    }

    out = value;

    return true;
}

bool JsonHelper::getVector3Array(
    std::vector<Vector3>& out,
    const char* name,
    const JsonObject& inObject
) {
    constexpr int VECTOR3_VALUE_COUNT = 3;

#ifdef _DEBUG
    if (!inObject.hasArray(name)) {
        return false;
    }
#endif // _DEBUG

    const auto& value = inObject.getArray(name);

    auto size = value.size();
    if (value.empty() || size % VECTOR3_VALUE_COUNT != 0) {
        return false;
    }

    auto sizeDiv3 = size / VECTOR3_VALUE_COUNT;
    out.resize(sizeDiv3);
    for (size_t i = 0; i < sizeDiv3; ++i) {
        out[i] = Vector3(
            std::stof(value[i * VECTOR3_VALUE_COUNT]),
            std::stof(value[i * VECTOR3_VALUE_COUNT + 1]),
            std::stof(value[i * VECTOR3_VALUE_COUNT + 2])
        );
    }

    return true;
}

void JsonHelper::setInt(
    int value,
    const char* name,
    JsonObject& inObject
) {
    //すでに値を保持しているなら上書き、ないなら追加
    auto str = std::to_string(value);
    if (inObject.hasValue(name)) {
        inObject.values.find(name)->second = str;
    } else {
        inObject.values.emplace(name, str);
    }
}

void JsonHelper::setFloat(
    float value,
    const char* name,
    JsonObject& inObject
) {
    auto str = std::to_string(value);
    if (inObject.hasValue(name)) {
        inObject.values.find(name)->second = str;
    } else {
        inObject.values.emplace(name, str);
    }
}

void JsonHelper::setString(
    const std::string& value,
    const char* name,
    JsonObject& inObject
) {
    if (inObject.hasValue(name)) {
        inObject.values.find(name)->second = value;
    } else {
        inObject.values.emplace(name, value);
    }
}

void JsonHelper::setBool(
    bool value,
    const char* name,
    JsonObject& inObject
) {
    auto str = (value) ? "true" : "false";
    if (inObject.hasValue(name)) {
        inObject.values.find(name)->second = str;
    } else {
        inObject.values.emplace(name, str);
    }
}

void JsonHelper::setVector2(
    const Vector2& value,
    const char* name,
    JsonObject& inObject
) {
    auto str = { std::to_string(value.x), std::to_string(value.y) };
    if (inObject.hasArray(name)) {
        inObject.valueArray.find(name)->second = str;
    } else {
        inObject.valueArray.emplace(name, str);
    }
}

void JsonHelper::setVector3(
    const Vector3& value,
    const char* name,
    JsonObject& inObject
) {
    auto str = { std::to_string(value.x), std::to_string(value.y), std::to_string(value.z) };
    if (inObject.hasArray(name)) {
        inObject.valueArray.find(name)->second = str;
    } else {
        inObject.valueArray.emplace(name, str);
    }
}

void JsonHelper::setVector4(
    const Vector4& value,
    const char* name,
    JsonObject& inObject
) {
    auto str = { std::to_string(value.x), std::to_string(value.y), std::to_string(value.z), std::to_string(value.w) };
    if (inObject.hasArray(name)) {
        inObject.valueArray.find(name)->second = str;
    } else {
        inObject.valueArray.emplace(name, str);
    }
}

void JsonHelper::setQuaternion(
    const Quaternion& value,
    const char* name,
    JsonObject& inObject
) {
    Vector4 tmp(value.x, value.y, value.z, value.w);
    setVector4(tmp, name, inObject);
}

void JsonHelper::setIntArray(
    const std::vector<int>& values,
    const char* name,
    JsonObject& inObject
) {
    auto size = values.size();
    if (inObject.hasArray(name)) {
        auto& target = inObject.valueArray.find(name)->second;
        target.resize(size);
        for (size_t i = 0; i < size; ++i) {
            target[i] = std::to_string(values[i]);
        }
    } else {
        auto& target = inObject.valueArray.emplace(name, JsonValueArray{}).first->second;
        target.resize(size);
        for (size_t i = 0; i < size; ++i) {
            target[i] = std::to_string(values[i]);
        }
    }
}

void JsonHelper::setStringArray(
    const std::vector<std::string>& values,
    const char* name,
    JsonObject& inObject
) {
    auto size = values.size();
    if (inObject.hasArray(name)) {
        inObject.valueArray.find(name)->second = values;
    } else {
        inObject.valueArray.emplace(name, values);
    }
}

void JsonHelper::setVector3Array(
    const std::vector<Vector3>& values,
    const char* name,
    JsonObject& inObject
) {
    auto size = values.size();
    if (inObject.hasArray(name)) {
        auto& target = inObject.valueArray.find(name)->second;
        target.resize(size * 3);
        for (size_t i = 0; i < size; ++i) {
            const auto& v = values[i];
            auto idx = i * 3;
            target[idx] = std::to_string(v.x);
            target[idx + 1] = std::to_string(v.y);
            target[idx + 2] = std::to_string(v.z);
        }
    } else {
        auto& target = inObject.valueArray.emplace(name, JsonValueArray{}).first->second;
        target.resize(size * 3);
        for (size_t i = 0; i < size; ++i) {
            const auto& v = values[i];
            auto idx = i * 3;
            target[idx] = std::to_string(v.x);
            target[idx + 1] = std::to_string(v.y);
            target[idx + 2] = std::to_string(v.z);
        }
    }
}
