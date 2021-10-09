#pragma once

#include <vector>

class JsonOutputStream {
public:
    JsonOutputStream();
    ~JsonOutputStream();
    JsonOutputStream(const JsonOutputStream&) = delete;
    JsonOutputStream& operator=(const JsonOutputStream&) = delete;

    //現在の書き込み位置に文字を追加する
    void push(char c);
    //容量を確保する
    void addSize(unsigned size);
    //文字列バッファを取得する
    const char* getStringBufer() const;

private:
    //出力用文字列バッファ
    std::vector<char> mStr;
    //現在の書き込み位置
    unsigned mWritePos;
};
