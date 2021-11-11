#include "PngReader.h"
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

    //圧縮データを解凍する
    if (!decode()) {
        return;
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
    in.read(reinterpret_cast<char*>(&mHeader.bitDepth), sizeof(mHeader.bitDepth));
    in.read(reinterpret_cast<char*>(&mHeader.colorType), sizeof(mHeader.colorType));
    in.read(reinterpret_cast<char*>(&mHeader.compression), sizeof(mHeader.compression));
    in.read(reinterpret_cast<char*>(&mHeader.filter), sizeof(mHeader.filter));
    in.read(reinterpret_cast<char*>(&mHeader.interlace), sizeof(mHeader.interlace));

    //CRCは必要ないので読み飛ばす
    skipCRC(in);
}

void PngReader::readImageData(std::ifstream& in, int length) {
    auto& data = mData.data;
    data.resize(length);
    in.read(reinterpret_cast<char*>(data.data()), length);

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

bool PngReader::decode() {
    z_stream z = { 0 };
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    z.next_in = mData.data.data();
    z.avail_in = mData.data.size();

    //初期化
    auto result = inflateInit(&z);
    if (result != Z_OK) {
        return false;
    }

    //解凍
    while (true) {
        //現在のサイズに一定量ずつ足して読み込んでいく
        mInflateData.resize(mInflateData.size() + READ_INFLATE_SIZE);

        z.next_out = &mInflateData[z.total_out];
        z.avail_out = READ_INFLATE_SIZE;
        result = inflate(&z, Z_NO_FLUSH);

        //Z_STREAM_ENDなら終了
        if (result == Z_STREAM_END) {
            break;
        }
        //Z_STREAM_ENDでなく、Z_OKでもなければエラー
        if (result != Z_OK) {
            return false;
        }
    }

    //実際に読み込んだサイズ分に補正する(実読み込み量のほうが小さいはず)
    assert(z.total_out <= mInflateData.size());
    mInflateData.resize(z.total_out);

    result = inflateEnd(&z);
    assert(result == Z_OK);

    return true;
}
