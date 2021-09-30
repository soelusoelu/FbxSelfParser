#pragma once

#include "FileMode.h"
#include "../Math/Math.h"
#include "../System/Json/JsonObject.h"
#include <string>
#include <vector>

//Jsonファイル解析ヘルパークラス
class JsonHelper {
public:
    //FileModeに応じてgetter/setter処理を分ける
    static void getSet(
        int& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        float& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        std::string& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        bool& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        Vector2& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        Vector3& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        Vector4& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        Quaternion& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        std::vector<int>& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        std::vector<std::string>& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );
    static void getSet(
        std::vector<Vector3>& value,
        const std::string& name,
        JsonObject& inObject,
        FileMode mode
    );

    //解析に成功したらtrue, 失敗したらfalse
    //true: nameから値をセット
    //false: 値を変えずにreturn
    static bool getInt(
        int& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getFloat(
        float& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getString(
        std::string& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getBool(
        bool& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getVector2(
        Vector2& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getVector3(
        Vector3& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getVector4(
        Vector4& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getQuaternion(
        Quaternion& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getIntArray(
        std::vector<int>& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getStringArray(
        std::vector<std::string>& out,
        const std::string& name,
        const JsonObject& inObject
    );
    static bool getVector3Array(
        std::vector<Vector3>& out,
        const std::string& name,
        const JsonObject& inObject
    );

    //setter
    static void setInt(
        int value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setFloat(
        float value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setString(
        const std::string& value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setBool(
        bool value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setVector2(
        const Vector2& value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setVector3(
        const Vector3& value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setVector4(
        const Vector4& value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setQuaternion(
        const Quaternion& value,
        const std::string& name,
        JsonObject& inObject
    );
    static void setIntArray(
        const std::vector<int>& values,
        const std::string& name,
        JsonObject& inObject
    );
    static void setStringArray(
        const std::vector<std::string>& values,
        const std::string& name,
        JsonObject& inObject
    );
    static void setVector3Array(
        const std::vector<Vector3>& values,
        const std::string& name,
        JsonObject& inObject
    );
};
