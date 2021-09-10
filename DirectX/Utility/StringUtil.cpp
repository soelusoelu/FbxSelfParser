#include "StringUtil.h"
#include "../Engine/DebugManager/DebugUtility/Debug.h"
#include "../System/SystemInclude.h"
#include <cassert>
#include <locale.h>
#include <iomanip>
#include <sstream>

std::vector<std::string> StringUtil::split(const std::string& src, const char delimiter) {
    StringArray temp;
    auto s = src;
    auto pos = s.find(delimiter);
    while (pos != std::string::npos) {
        auto sub = s.substr(0, pos);
        temp.emplace_back(sub);
        s = s.substr(pos + 1);
        pos = s.find(delimiter);
    }
    temp.emplace_back(s);

    return temp;
}

std::wstring StringUtil::charToWchar(const std::string& src) {
    //文字数を取得する
    auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, src.c_str(), -1, nullptr, 0);

    std::wstring dst;
    dst.resize(num1);
    //文字列を取得する
    auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, src.c_str(), -1, dst.data(), num1);

    //一応チェック
    assert(num1 == num2);

    return dst;
}

std::string StringUtil::wcharToString(const std::wstring& src) {
    //文字数を取得する
    auto num1 = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, nullptr, 0, nullptr, nullptr);

    std::string dst;
    //ヌル文字削除のために-1
    dst.resize(num1 - 1);
    //文字列を取得する
    auto num2 = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, dst.data(), num1, nullptr, nullptr);

    //一応チェック
    assert(num1 == num2);

    return dst;
}

std::string StringUtil::boolToString(bool value) {
    return (value) ? "true" : "false";
}

std::string StringUtil::intToString(int number) {
    return std::to_string(number);
}

std::string StringUtil::floatToString(float number, int decimalDigits) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalDigits) << number;
    return oss.str();
}

std::string StringUtil::vector2ToString(const Vector2& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    return x + "  " + y;
}

std::string StringUtil::vector3ToString(const Vector3& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    auto z = "Z " + floatToString(vec.z, decimalDigits);
    return x + "  " + y + "  " + z;
}

std::string StringUtil::vector4ToString(const Vector4& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    auto z = "Z " + floatToString(vec.z, decimalDigits);
    auto w = "W " + floatToString(vec.w, decimalDigits);
    return x + "  " + y + "  " + z + "  " + w;
}

std::string StringUtil::quaternionToString(const Quaternion& quaternion, int decimalDigits) {
    auto x = "X " + floatToString(quaternion.x, decimalDigits);
    auto y = "Y " + floatToString(quaternion.y, decimalDigits);
    auto z = "Z " + floatToString(quaternion.z, decimalDigits);
    auto w = "W " + floatToString(quaternion.w, decimalDigits);
    return x + "  " + y + "  " + z + "  " + w;
}

std::string StringUtil::anyToString(const std::any& src) {
    if (!src.has_value()) {
        return "";
    }

    const auto& type = src.type();
    std::string str;
    if (type == typeid(int)) {
        auto value = std::any_cast<int>(src);
        str = intToString(value);
    } else if (type == typeid(float)) {
        auto value = std::any_cast<float>(src);
        str = floatToString(value);
    } else if (type == typeid(bool)) {
        auto value = std::any_cast<bool>(src);
        str = boolToString(value);
    } else if (type == typeid(const char*)) {
        str = std::any_cast<const char*>(src);
    } else if (type == typeid(std::string)) {
        str = std::any_cast<std::string>(src);
    } else if (type == typeid(Vector2)) {
        auto value = std::any_cast<Vector2>(src);
        str = vector2ToString(value);
    } else if (type == typeid(Vector3)) {
        auto value = std::any_cast<Vector3>(src);
        str = vector3ToString(value);
    } else if (type == typeid(Vector4)) {
        auto value = std::any_cast<Vector4>(src);
        str = vector4ToString(value);
    } else if (type == typeid(Quaternion)) {
        auto value = std::any_cast<Quaternion>(src);
        str = quaternionToString(value);
    } else {
        Debug::logWarning(src.type().name());
    }

    return str;
}

std::string StringUtil::utf8ToShitJIS(const std::string& utf8) {
    //Unicodeへ変換後の文字列長を得る
    int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size() + 1, NULL, 0);

    //必要な分だけUnicode文字列のバッファを確保
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //UTF8からUnicodeへ変換
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size() + 1, bufUnicode, lenghtUnicode);

    //ShiftJISへ変換後の文字列長を得る
    int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //必要な分だけShiftJIS文字列のバッファを確保
    char* bufShiftJis = new char[lengthSJis];

    //UnicodeからShiftJISへ変換
    WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, bufShiftJis, lengthSJis, NULL, NULL);

    std::string strSJis(bufShiftJis);

    delete[] bufUnicode;
    delete[] bufShiftJis;

    return strSJis;
}

std::string StringUtil::shitJISToUtf8(const std::string& shiftJIS) {
    //Unicodeへ変換後の文字列長を得る
    int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, shiftJIS.c_str(), shiftJIS.size() + 1, NULL, 0);

    //必要な分だけUnicode文字列のバッファを確保
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //ShiftJISからUnicodeへ変換
    MultiByteToWideChar(CP_THREAD_ACP, 0, shiftJIS.c_str(), shiftJIS.size() + 1, bufUnicode, lenghtUnicode);

    //UTF8へ変換後の文字列長を得る
    int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //必要な分だけUTF8文字列のバッファを確保
    char* bufUTF8 = new char[lengthUTF8];

    //UnicodeからUTF8へ変換
    WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, bufUTF8, lengthUTF8, NULL, NULL);

    std::string strUTF8(bufUTF8);

    delete[] bufUnicode;
    delete[] bufUTF8;

    return strUTF8;
}
