#pragma once

#include <memory>
#include <string>
#include <vector>

struct JsonObject;

//JsonValueの保持している要素を表す列挙型
enum class JsonValueFlag {
    NONE,
    BOOL,
    INT,
    FLOAT,
    STRING,
    ARRAY,
    OBJECT
};

//JsonValueで使用する数値構造体
struct JsonNumber {
    int i = 0;
    float f = 0.f;
};

//Jsonファイルの値を表す構造体
//ファイルの値1つがこの構造体に入る(1つしか入らない)
struct JsonValue {
    bool b; //真偽値
    JsonNumber n; //数値
    std::string s; //文字列
    std::vector<JsonValue> a; //配列
    std::shared_ptr<JsonObject> o; //子オブジェクト
    JsonValueFlag f; //どの型を格納したか

    JsonValue();
    JsonValue(JsonValueFlag flag);
    JsonValue(const JsonValue& rhs);
    JsonValue& operator=(const JsonValue& rhs);

    //真偽値を取得する
    bool getBool() const;
    //整数を取得する
    int getInt() const;
    //小数を取得する
    float getFloat() const;
    //文字列を取得する
    const std::string& getString() const;
    //配列を取得する
    const std::vector<JsonValue>& getArray() const;
    //子オブジェクトを取得する
    JsonObject& getObject() const;

    //真偽値を格納する
    void setBool(bool value);
    //整数を格納する
    void setInt(int value);
    //小数を格納する
    void setFloat(float value);
    //文字列を格納する
    void setString(const std::string& value);
    //配列用要素に設定する
    const std::vector<JsonValue>& setArray();
    //オブジェクト用要素に設定する
    JsonObject& setObject();

    //配列の末尾に真偽値を追加する
    void pushBack(bool value);
    //配列の末尾に整数を追加する
    void pushBack(int value);
    //配列の末尾に小数を追加する
    void pushBack(float value);
    //配列の末尾に文字列を追加する
    void pushBack(const std::string& value);

    //格納している値が真偽値か
    bool isBool() const;
    //格納している値が整数か
    bool isInt() const;
    //格納している値が小数か
    bool isFloat() const;
    //格納している値が文字列か
    bool isString() const;
    //格納している値が配列か
    bool isArray() const;
    //格納している値がオブジェクトか
    bool isObject() const;
};
