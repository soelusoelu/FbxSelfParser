#pragma once

struct ImageTrailer {
    //ChunkDataのサイズ、固定0
    static constexpr char LENGTH = 0;
    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 'I', 'E', 'N', 'D' };

    //ImageTrailerか判断する
    static bool isType(const char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
