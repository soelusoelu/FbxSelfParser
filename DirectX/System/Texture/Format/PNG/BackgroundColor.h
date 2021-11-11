#pragma once

struct BackgroundColor {
    unsigned char paletteIndex;
    unsigned short grayLevel;
    unsigned short r;
    unsigned short g;
    unsigned short b;

    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 'b', 'K', 'G', 'D' };

    //TextureDataか判断する
    static bool isType(const unsigned char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
