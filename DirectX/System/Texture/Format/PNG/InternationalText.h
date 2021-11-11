#pragma once

struct InternationalText {
    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 'i', 'T', 'X', 't' };

    //TextureDataか判断する
    static bool isType(const unsigned char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
