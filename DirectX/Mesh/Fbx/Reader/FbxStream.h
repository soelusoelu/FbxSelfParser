#pragma once

#include <vector>

class FbxStream {
public:
    FbxStream(const char* filePath);
    ~FbxStream();
    FbxStream(const FbxStream&) = delete;
    FbxStream& operator=(const FbxStream&) = delete;

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
