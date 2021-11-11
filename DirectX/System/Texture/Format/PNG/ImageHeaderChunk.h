#pragma once

#include <cassert>

struct ImageHeaderChunk {
    enum class ColorType {
        GRAYSCALE, //グレースケール
        TRUE_COLOR, //RGB
        INDEXED_COLOR, //パレットインデックス
        GRAYSCALE_ALPHA, //グレースケール + アルファ
        TRUE_COLOR_ALPHA //RGB + アルファ
    };

    //画像の幅
    unsigned width;
    //画像の高さ
    unsigned height;
    //ビット深度
    unsigned char bitDepth;
    //カラータイプ
    unsigned char colorType;
    //圧縮手法
    unsigned char compression;
    //フィルター手法
    unsigned char filter;
    //インターレース手法
    unsigned char interlace;

    //ChunkDataのサイズ、固定13
    static constexpr char LENGTH = 13;
    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 'I', 'H', 'D', 'R' };

    //画像のカラータイプを取得する
    ColorType getColorType() const {
        if (colorType == 0) {
            assert(bitDepth == 1 || bitDepth == 2 || bitDepth == 4 || bitDepth == 8 || bitDepth == 16);
            return ColorType::GRAYSCALE;
        } else if (colorType == 2) {
            assert(bitDepth == 8 || bitDepth == 16);
            return ColorType::TRUE_COLOR;
        } else if (colorType == 3) {
            assert(bitDepth == 1 || bitDepth == 2 || bitDepth == 4 || bitDepth == 8);
            return ColorType::INDEXED_COLOR;
        } else if (colorType == 4) {
            assert(bitDepth == 8 || bitDepth == 16);
            return ColorType::GRAYSCALE_ALPHA;
        } else if (colorType == 6) {
            assert(bitDepth == 8 || bitDepth == 16);
            return ColorType::TRUE_COLOR_ALPHA;
        } else {
            assert(false);
        }
    }

    //1ピクセルの表示に必要なbit数を取得する
    char getBitsPerPixel() const {
        if (colorType == 0) {
            return bitDepth;
        } else if (colorType == 2) {
            return bitDepth * 3;
        } else if (colorType == 3) {
            return bitDepth;
        } else if (colorType == 4) {
            return bitDepth * 2;
        } else if (colorType == 6) {
            return bitDepth * 4;
        } else {
            assert(false);
        }
    }

    //ImageHeaderChunkか判断する
    static bool isType(const unsigned char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
