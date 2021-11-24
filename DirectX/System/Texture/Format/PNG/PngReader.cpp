#include "PngReader.h"
#include "BackgroundColor.h"
#include "ImageLastModificationTime.h"
#include "ImageTrailer.h"
#include "InflateDataToColorConverter.h"
#include "InternationalText.h"
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

void PngReader::read(
    const std::string& filePath,
    std::vector<unsigned char>& colors
) {
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
        unsigned length = 0;
        inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
        length = byteSwap(length);

        unsigned char type[4] = { 0 };
        inFile.read(reinterpret_cast<char*>(type), sizeof(type));

        //typeがImageTrailerだったら終了
        if (ImageTrailer::isType(type)) {
            break;
        } else if (PhysicalPixelDimension::isType(type)) {
            readPhysicalPixelDimension(inFile, length);
        } else if (TextureData::isType(type)) {
            readTextureData(inFile, length);
        } else if (ImageData::isType(type)) {
            readImageData(inFile, length);
        } else if (BackgroundColor::isType(type)) {
            readBackgroundColor(inFile, length);
        } else if (ImageLastModificationTime::isType(type)) {
            readImageLastModificationTime(inFile, length);
        } else if (InternationalText::isType(type)) {
            readInternationalText(inFile, length);
        } else {
            assert(false);
        }
    }

    //圧縮データを解凍する
    if (!decode()) {
        return;
    }

    //解凍したデータをRGBAに変換する
    InflateDataToColorConverter converter;
    converter.convert(colors, mInflateData, mHeader);
}

unsigned PngReader::getWidth() const {
    return mHeader.width;
}

unsigned PngReader::getHeight() const {
    return mHeader.height;
}

bool PngReader::isPng(std::ifstream& in) const {
    unsigned char sig[8] = { 0 };
    in.read(reinterpret_cast<char*>(sig), sizeof(sig));

    //sig[0]は非ASCII文字なのでチェックしない
    if (sig[1] == 0x50 && sig[2] == 0x4E && sig[3] == 0x47
        && sig[4] == 0x0D && sig[5] == 0x0A && sig[6] == 0x1A && sig[7] == 0x0A) {
        return true;
    }

    return false;
}

void PngReader::readImageHeaderChunk(std::ifstream& in) {
    unsigned length = 0;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    assert(byteSwap(length) == ImageHeaderChunk::LENGTH);

    unsigned char type[4] = { 0 };
    in.read(reinterpret_cast<char*>(type), sizeof(type));
    assert(ImageHeaderChunk::isType(type));

    unsigned char bytes[ImageHeaderChunk::LENGTH] = { 0 };
    in.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));

    mHeader.width = getByte<unsigned, unsigned char>(bytes, 0);
    mHeader.width = byteSwap(mHeader.width);
    mHeader.height = getByte<unsigned, unsigned char>(bytes, 4);
    mHeader.height = byteSwap(mHeader.height);
    mHeader.bitDepth = getByte<unsigned char, unsigned char>(bytes, 8);
    mHeader.colorType = getByte<unsigned char, unsigned char>(bytes, 9);
    mHeader.compression = getByte<unsigned char, unsigned char>(bytes, 10);
    mHeader.filter = getByte<unsigned char, unsigned char>(bytes, 11);
    mHeader.interlace = getByte<unsigned char, unsigned char>(bytes, 12);

    //CRCは必要ないので読み飛ばす
    skipCRC(in);
}

void PngReader::readImageData(std::ifstream& in, int length) {
    auto& data = mData.data;
    //複数回呼ばれる可能性があるため付き足していく
    auto prevSize = data.size();
    data.resize(prevSize + length);
    in.read(reinterpret_cast<char*>(&data[prevSize]), length);

    //CRCは必要ないので読み飛ばす
    skipCRC(in);
}

void PngReader::readPhysicalPixelDimension(std::ifstream& in, int length) const {
    assert(length == PhysicalPixelDimension::LENGTH);

    //必要ないので読み込まずスキップ
    in.seekg(length, std::ios_base::cur);

    //PhysicalPixelDimension ppd = { 0 };
    //unsigned char bytes[PhysicalPixelDimension::LENGTH] = { 0 };
    //in.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));

    //ppd.numPixelX = getByte<unsigned, unsigned char>(bytes, 0);
    //ppd.numPixelX = byteSwap(ppd.numPixelX);
    //ppd.numPixelY = getByte<unsigned, unsigned char>(bytes, 4);
    //ppd.numPixelY = byteSwap(ppd.numPixelY);
    //ppd.unit = getByte<unsigned char, unsigned char>(bytes, 8);

    //CRCは必要ないので読み飛ばす
    skipCRC(in);
}

void PngReader::readTextureData(std::ifstream& in, int length) const {
    //必要ないので読み込まずスキップ
    in.seekg(length, std::ios_base::cur);
    skipCRC(in);
}

void PngReader::readInternationalText(std::ifstream& in, int length) const {
    //必要ないので読み込まずスキップ
    in.seekg(length, std::ios_base::cur);
    skipCRC(in);
}

void PngReader::readBackgroundColor(std::ifstream& in, int length) const {
    //必要ないので読み込まずスキップ
    //in.seekg(length, std::ios_base::cur);

    BackgroundColor bc = { 0 };
    if (mHeader.colorType == 3) {
        in.read(reinterpret_cast<char*>(&bc.paletteIndex), sizeof(bc.paletteIndex));
    } else if (mHeader.colorType == 0 || mHeader.colorType == 4) {
        in.read(reinterpret_cast<char*>(&bc.grayLevel), sizeof(bc.grayLevel));
        bc.grayLevel = byteSwap(bc.grayLevel);
    } else if (mHeader.colorType == 2 || mHeader.colorType == 6) {
        in.read(reinterpret_cast<char*>(&bc.r), sizeof(bc.r));
        bc.r = byteSwap(bc.r);
        in.read(reinterpret_cast<char*>(&bc.g), sizeof(bc.g));
        bc.g = byteSwap(bc.g);
        in.read(reinterpret_cast<char*>(&bc.b), sizeof(bc.b));
        bc.b = byteSwap(bc.b);
    } else {
        assert(false);
    }

    skipCRC(in);
}

void PngReader::readImageLastModificationTime(std::ifstream& in, int length) const {
    assert(length == ImageLastModificationTime::LENGTH);

    //必要ないので読み込まずスキップ
    //in.seekg(length, std::ios_base::cur);

    ImageLastModificationTime ilmt = { 0 };
    unsigned char bytes[ImageLastModificationTime::LENGTH] = { 0 };
    in.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));

    ilmt.year = getByte<unsigned short, unsigned char>(bytes, 0);
    ilmt.year = byteSwap(ilmt.year);
    ilmt.month = getByte<unsigned char, unsigned char>(bytes, 2);
    ilmt.day = getByte<unsigned char, unsigned char>(bytes, 3);
    ilmt.hour = getByte<unsigned char, unsigned char>(bytes, 4);
    ilmt.minute = getByte<unsigned char, unsigned char>(bytes, 5);
    ilmt.second = getByte<unsigned char, unsigned char>(bytes, 6);

    //CRCは必要ないので読み飛ばす
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

    //解凍後のデータサイズは
    //横幅 * 縦幅 * 使用する色の種類数(rgbaのうち使用する数、基本4) + 縦幅
    auto usedColorNum = mHeader.getBitsPerPixel() / mHeader.bitDepth;
    mInflateData.resize(mHeader.width * mHeader.height * usedColorNum + mHeader.height);

    z.next_out = mInflateData.data();

    //解凍
    while (true) {
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

    //読み込み量と等しいかチェック
    assert(z.total_out == mInflateData.size());

    //終了処理
    result = inflateEnd(&z);
    assert(result == Z_OK);

    return true;
}
