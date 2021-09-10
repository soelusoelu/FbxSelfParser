#include "JsonHelper.h"

void JsonHelper::getSet(
    int& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setInt(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getInt(value, name, inObject);
    }
}

void JsonHelper::getSet(
    float& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setFloat(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getFloat(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::string& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setString(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getString(value, name, inObject);
    }
}

void JsonHelper::getSet(
    bool& value,
    const char* name,
    rapidjson::Value& inObject, rapidjson::Document::AllocatorType& alloc, FileMode mode)
{
    if (mode == FileMode::SAVE) {
        setBool(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getBool(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Vector2& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector2(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getVector2(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Vector3& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector3(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getVector3(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Vector4& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector4(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getVector4(value, name, inObject);
    }
}

void JsonHelper::getSet(
    Quaternion& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setQuaternion(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getQuaternion(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::vector<int>& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setIntArray(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getIntArray(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::vector<std::string>& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setStringArray(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getStringArray(value, name, inObject);
    }
}

void JsonHelper::getSet(
    std::vector<Vector3>& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc,
    FileMode mode
) {
    if (mode == FileMode::SAVE) {
        setVector3Array(value, name, inObject, alloc);
    } else if (mode == FileMode::LOAD) {
        getVector3Array(value, name, inObject);
    }
}

bool JsonHelper::getInt(
    int& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    //プロパティが存在するか
    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    //値の方を取得し、整数であることを確認
    auto& value = itr->value;
    if (!value.IsInt()) {
        return false;
    }

    //プロパティの取得に成功
    out = value.GetInt();
    return true;
}

bool JsonHelper::getFloat(
    float& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsDouble()) {
        return false;
    }

    out = static_cast<float>(value.GetDouble());
    return true;
}

bool JsonHelper::getString(
    std::string& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsString()) {
        return false;
    }

    out = value.GetString();
    return true;
}

bool JsonHelper::getBool(
    bool& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsBool()) {
        return false;
    }

    out = value.GetBool();
    return true;
}

bool JsonHelper::getVector2(
    Vector2& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    constexpr int VECTOR2_VALUE_COUNT = 2;

    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsArray() || value.Size() != VECTOR2_VALUE_COUNT) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < VECTOR2_VALUE_COUNT; ++i) {
        if (!value[i].IsDouble()) {
            return false;
        }
    }

    out.x = static_cast<float>(value[0].GetDouble());
    out.y = static_cast<float>(value[1].GetDouble());

    return true;
}

bool JsonHelper::getVector3(
    Vector3& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    constexpr int VECTOR3_VALUE_COUNT = 3;

    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsArray() || value.Size() != VECTOR3_VALUE_COUNT) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < VECTOR3_VALUE_COUNT; ++i) {
        if (!value[i].IsDouble()) {
            return false;
        }
    }

    out.x = static_cast<float>(value[0].GetDouble());
    out.y = static_cast<float>(value[1].GetDouble());
    out.z = static_cast<float>(value[2].GetDouble());

    return true;
}

bool JsonHelper::getVector4(
    Vector4& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    constexpr int VECTOR4_VALUE_COUNT = 4;

    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsArray() || value.Size() != VECTOR4_VALUE_COUNT) {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < VECTOR4_VALUE_COUNT; ++i) {
        if (!value[i].IsDouble()) {
            return false;
        }
    }

    out.x = static_cast<float>(value[0].GetDouble());
    out.y = static_cast<float>(value[1].GetDouble());
    out.z = static_cast<float>(value[2].GetDouble());
    out.w = static_cast<float>(value[3].GetDouble());

    return true;
}

bool JsonHelper::getQuaternion(
    Quaternion& out,
    const char* name,
    const rapidjson::Value& inObject
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
    const rapidjson::Value& inObject
) {
    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsArray() || value.Empty()) {
        return false;
    }

    auto size = static_cast<size_t>(value.Size());
    std::vector<int> temp(size);
    for (size_t i = 0; i < size; ++i) {
        if (!value[i].IsInt()) {
            return false;
        }
        temp[i] = value[i].GetInt();
    }

    out = temp;

    return true;
}

bool JsonHelper::getStringArray(
    std::vector<std::string>& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    if (!value.IsArray() || value.Empty()) {
        return false;
    }

    auto size = static_cast<size_t>(value.Size());
    std::vector<std::string> temp(size);
    for (size_t i = 0; i < size; ++i) {
        if (!value[i].IsString()) {
            return false;
        }
        temp[i] = value[i].GetString();
    }

    out = temp;

    return true;
}

bool JsonHelper::getVector3Array(
    std::vector<Vector3>& out,
    const char* name,
    const rapidjson::Value& inObject
) {
    constexpr int VECTOR3_VALUE_COUNT = 3;

    auto itr = inObject.FindMember(name);
    if (itr == inObject.MemberEnd()) {
        return false;
    }

    auto& value = itr->value;
    auto size = static_cast<size_t>(value.Size());
    if (!value.IsArray() || value.Empty() || size % VECTOR3_VALUE_COUNT != 0) {
        return false;
    }

    for (size_t i = 0; i < size; ++i) {
        if (!value[i].IsDouble()) {
            return false;
        }
    }

    size = value.Size() / VECTOR3_VALUE_COUNT;
    out.resize(size);
    for (size_t i = 0; i < size; ++i) {
        out[i] = Vector3(
            value[i * VECTOR3_VALUE_COUNT].GetDouble(),
            value[i * VECTOR3_VALUE_COUNT + 1].GetDouble(),
            value[i * VECTOR3_VALUE_COUNT + 2].GetDouble()
        );
    }

    return true;
}

void JsonHelper::setInt(
    int value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    rapidjson::Value v(value);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setFloat(
    float value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    rapidjson::Value v(value);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setString(
    const std::string& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    rapidjson::Value v;
    v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setBool(
    bool value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    rapidjson::Value v(value);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector2(
    const Vector2& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, zそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);

    //inObjectに配列として追加
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector3(
    const Vector3& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, zそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);

    //inObjectに配列として追加
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector4(
    const Vector4& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, z, wそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    v.PushBack(rapidjson::Value(value.w).Move(), alloc);

    //inObjectに配列として追加
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setQuaternion(
    const Quaternion& value,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);
    //x, y, z, wそれぞれ追加
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    v.PushBack(rapidjson::Value(value.w).Move(), alloc);

    //inObjectに配列として追加
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setIntArray(
    const std::vector<int>& values,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);

    //配列要素を順に追加していく
    for (const auto& value : values) {
        rapidjson::Value temp(value);
        v.PushBack(temp, alloc);
    }

    //inObjectに配列として追加
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setStringArray(
    const std::vector<std::string>& values,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);

    //配列要素を順に追加していく
    rapidjson::Value temp;
    for (const auto& value : values) {
        temp.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
        v.PushBack(temp, alloc);
    }

    //inObjectに配列として追加
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::setVector3Array(
    const std::vector<Vector3>& values,
    const char* name,
    rapidjson::Value& inObject,
    rapidjson::Document::AllocatorType& alloc
) {
    //配列を生成
    rapidjson::Value v(rapidjson::kArrayType);

    for (const auto& value : values) {
        //x, y, zそれぞれ追加
        v.PushBack(rapidjson::Value(value.x).Move(), alloc);
        v.PushBack(rapidjson::Value(value.y).Move(), alloc);
        v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    }

    //inObjectに配列として追加
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
