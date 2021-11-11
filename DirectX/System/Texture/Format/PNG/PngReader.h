#pragma once

#include "ImageData.h"
#include "ImageHeaderChunk.h"
#include "../ITextureReader.h"
#include <vector>

#ifndef ZLIB_WINAPI
#define ZLIB_WINAPI
#endif // !ZLIB_WINAPI

#include <zlib.h>

#pragma comment(lib, "../Library/zlib/lib/zlibstat.lib")

class PngReader
    : public ITextureReader
{
public:
    PngReader();
    ~PngReader();
    PngReader(const PngReader&) = delete;
    PngReader& operator=(const PngReader&) = delete;

    virtual void read(const std::string& filePath) override;

private:
    //PNGファイルか判断する
    bool isPng(std::ifstream& in) const;

    //ImageHeaderChunkを読み込む
    void readImageHeaderChunk(std::ifstream& in);
    //ImageDataを読み込む
    void readImageData(std::ifstream& in, int length);
    //PhysicalPixelDimensionを読み込む
    void readPhysicalPixelDimension(std::ifstream& in, int length) const;
    //TextureDataを読み込む
    void readTextureData(std::ifstream& in, int length) const;

    //CRC分を読み飛ばす
    void skipCRC(std::ifstream& in) const;

    bool decode();

private:
    ImageHeaderChunk mHeader;
    ImageData mData;
    std::vector<unsigned char> mInflateData;
    std::vector<unsigned char> mRgba;

    static constexpr int READ_INFLATE_SIZE = 256;
};
