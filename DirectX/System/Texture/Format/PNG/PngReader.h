#pragma once

#include "ImageHeaderChunk.h"
#include "../ITextureReader.h"
#include <vector>

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
    void readImageData(std::ifstream& in, int length) const;
    //PhysicalPixelDimensionを読み込む
    void readPhysicalPixelDimension(std::ifstream& in, int length) const;
    //TextureDataを読み込む
    void readTextureData(std::ifstream& in, int length) const;

    //CRC分を読み飛ばす
    void skipCRC(std::ifstream& in) const;

private:
    ImageHeaderChunk mHeader;
    std::vector<unsigned char> mRgba;
};
