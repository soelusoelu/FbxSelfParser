#include "FbxStream.h"
#include <fstream>
#include <cassert>

FbxStream::FbxStream(const char* filePath)
    : mReadPos(0)
{
    //バイナリモードで開き、末尾に移動
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        assert(false);
        return;
    }

    //ファイルの末尾 = ファイルサイズ
    auto fileSize = file.tellg();
    //ファイルの先頭まで戻る
    file.seekg(0, std::ios::beg);

    //末尾のヌルを含めたsize+1の配列を作る
    mFileStr.resize(static_cast<size_t>(fileSize) + 1);
    //配列にバイト列を書き込む
    file.read(mFileStr.data(), static_cast<size_t>(fileSize));
}

FbxStream::~FbxStream() = default;

char FbxStream::peek() const {
    return mFileStr[mReadPos];
}

char FbxStream::take() {
    ++mReadPos;
    return mFileStr[mReadPos];
}
