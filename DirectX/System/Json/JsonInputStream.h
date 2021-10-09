#pragma once

#include <vector>

class JsonInputStream {
public:
    JsonInputStream(const char* filePath);
    ~JsonInputStream();
    JsonInputStream(const JsonInputStream&) = delete;
    JsonInputStream& operator=(const JsonInputStream&) = delete;

    //現在の読み取り位置の文字を返す
    char peek() const;
    //現在の読み取り位置の次の文字を返す
    char take();

private:
    //ファイル全体の文字列
    std::vector<char> mFileStr;
    //現在の読み取り位置
    unsigned mReadPos;
};
