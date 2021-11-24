#pragma once

struct PhysicalPixelDimension {
    //X軸上の単位あたりのピクセル数
    unsigned numPixelX;
    //Y軸上の単位あたりのピクセル数
    unsigned numPixelY;
    //単位
    //0: 不明
    //1: メートル
    unsigned char unit;

    //ChunkDataのサイズ、固定9
    static constexpr char LENGTH = 9;
    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 'p', 'H', 'Y', 's' };

    //PhysicalPixelDimensionか判断する
    static bool isType(const unsigned char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
