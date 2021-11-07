#include "PngReader.h"
#include "ImageData.h"
#include "ImageTrailer.h"
#include "PhysicalPixelDimension.h"
#include "TextureData.h"
#include "../../../GlobalFunction.h"
#include <cassert>
#include <fstream>

PngReader::PngReader()
    : mHeader{ 0 }
{
}

PngReader::~PngReader() = default;

void PngReader::read(const std::string& filePath) {
    //ファイルを開く
    std::ifstream inFile;
    inFile.open(filePath, std::ios_base::in | std::ios_base::binary);
    assert(inFile.is_open());

    //PNGファイルかチェック
    bool result = isPng(inFile);
    assert(result);

    //ImageHeaderChunkを読み込む
    readImageHeaderChunk(inFile);

    while (true) {
        int length = 0;
        inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
        length = byteSwap(length);

        char type[4] = { 0 };
        inFile.read(type, sizeof(type));

        //typeがImageTrailerだったら終了
        if (ImageTrailer::isType(type)) {
            break;
        } else if (PhysicalPixelDimension::isType(type)) {
            readPhysicalPixelDimension(inFile, length);
        } else if (TextureData::isType(type)) {
            readTextureData(inFile, length);
        } else if (ImageData::isType(type)) {
            readImageData(inFile, length);
        } else {
            assert(false);
        }
    }
}

bool PngReader::isPng(std::ifstream& in) const {
    char sig[8] = { 0 };
    in.read(sig, sizeof(sig));

    //sig[0]は非ASCII文字なのでチェックしない
    if (sig[1] == 0x50 && sig[2] == 0x4E && sig[3] == 0x47
        && sig[4] == 0x0D && sig[5] == 0x0A && sig[6] == 0x1A && sig[7] == 0x0A) {
        return true;
    }

    return false;
}

void PngReader::readImageHeaderChunk(std::ifstream& in) {
    int length = 0;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    assert(byteSwap(length) == ImageHeaderChunk::LENGTH);

    char type[4] = { 0 };
    in.read(type, sizeof(type));
    assert(ImageHeaderChunk::isType(type));

    in.read(reinterpret_cast<char*>(&mHeader.width), sizeof(mHeader.width));
    mHeader.width = byteSwap(mHeader.width);
    in.read(reinterpret_cast<char*>(&mHeader.height), sizeof(mHeader.height));
    mHeader.height = byteSwap(mHeader.height);
    in.read(&mHeader.bitDepth, sizeof(mHeader.bitDepth));
    in.read(&mHeader.colorType, sizeof(mHeader.colorType));
    in.read(&mHeader.compression, sizeof(mHeader.compression));
    in.read(&mHeader.filter, sizeof(mHeader.filter));
    in.read(&mHeader.interlace, sizeof(mHeader.interlace));

    //CRCは必要ないので読み飛ばす
    skipCRC(in);
}

void PngReader::readImageData(std::ifstream& in, int length) const {
    ImageData id;
    id.data.resize(length);
    in.read(id.data.data(), length);

    //CRCは必要ないので読み飛ばす
    skipCRC(in);
}

void PngReader::readPhysicalPixelDimension(std::ifstream& in, int length) const {
    assert(length == PhysicalPixelDimension::LENGTH);

    PhysicalPixelDimension ppd = { 0 };
    in.read(reinterpret_cast<char*>(&ppd.numPixelX), sizeof(ppd.numPixelX));
    ppd.numPixelX = byteSwap(ppd.numPixelX);
    in.read(reinterpret_cast<char*>(&ppd.numPixelY), sizeof(ppd.numPixelY));
    ppd.numPixelY = byteSwap(ppd.numPixelY);
    in.read(&ppd.unit, sizeof(ppd.unit));

    //CRCは必要ないので読み飛ばす
    skipCRC(in);
}

void PngReader::readTextureData(std::ifstream& in, int length) const {
    //必要ないので読み込まずスキップ
    in.seekg(length, std::ios_base::cur);
    skipCRC(in);
}

void PngReader::skipCRC(std::ifstream& in) const {
    in.seekg(4, std::ios_base::cur);
}
