#pragma once

#include "FbxRootValue.h"
#include "FbxValue.h"
#include <string>

class FbxStream;

class FbxReader {
public:
    //FbxStreamの中身を解析する
    void parse(FbxStream& in, FbxValue& value) const;

private:
    //attributeかvalueか判断し解析する
    void parseAttributesOrValue(FbxStream& in, FbxValue& value, const std::string& key) const;
    //attributeを解析する
    void parseAttributes(FbxStream& in, FbxValue& value, const std::string& key) const;
    //{}で囲まれたオブジェクトを解析する
    void parseObject(FbxStream& in, FbxValue& out) const;
    //値名を解析する
    void parseValueName(FbxStream& in, std::string& out) const;
    //数値を解析する
    void parseNumber(FbxStream& in, std::string& out) const;
    //""で囲まれた文字列を解析する
    void parseString(FbxStream& in, std::string& out) const;
    //スペースとコメントをスキップする
    void skipSpaceAndComments(FbxStream& in) const;
    //スペースをスキップする
    void skipSpace(FbxStream& in) const;
    //expectが現在の読み取り位置の文字と一致していたら、読み取り位置を一つ進める
    bool consume(FbxStream& in, const char expect) const;
};
