#pragma once

#include "FbxObject.h"
#include <string>

class FbxStream;

class FbxReader {
public:
    //FbxStreamの中身を解析する
    void parse(FbxStream& in, FbxObject& value) const;

private:
    //attributeかvalueか判断し解析する
    void parseAttributesOrValue(FbxStream& in, FbxObject& value, const std::string& key) const;
    //値を解析する
    void parseValue(FbxStream& in, std::string& value) const;
    //{}で囲まれたオブジェクトを解析する
    void parseObject(FbxStream& in, FbxObject& out) const;
    //値名を解析する
    void parseValueName(FbxStream& in, std::string& out) const;
    //数値を解析する
    void parseNumber(FbxStream& in, std::string& out) const;
    //数値、配列なら数値配列を解析する
    void parseNumberArray(FbxStream& in, std::string& out) const;
    //""で囲まれた文字列を解析する
    void parseString(FbxStream& in, std::string& out) const;
    //配列を解析する
    void parseArray(FbxStream& in, FbxObject& out, const std::string& key) const;
    //Properties70を解析する
    void parseProperties70(FbxStream& in, FbxObject& out) const;
    //Properties70の値を解析する
    void parseProperties70Value(FbxStream& in, FbxProperties70& out) const;
    //スペースとコメントをスキップする
    void skipSpaceAndComments(FbxStream& in) const;
    //スペースをスキップする
    void skipSpace(FbxStream& in) const;
    //expectが現在の読み取り位置の文字と一致していたら、読み取り位置を一つ進める
    bool consume(FbxStream& in, char expect) const;
    //attributeを追加する対象のオブジェクトを取得する
    FbxObject& getObjectAddAttribute(FbxObject& value) const;
};
