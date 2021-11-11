#pragma once

struct ImageLastModificationTime {
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;

    //ChunkDataのサイズ、固定7
    static constexpr char LENGTH = 7;
    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 't', 'I', 'M', 'E' };

    //TextureDataか判断する
    static bool isType(const unsigned char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
