#pragma once

#include <vector>

struct ImageData {
    //イメージデータ
    std::vector<unsigned char> data;

    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 'I', 'D', 'A', 'T' };

    //ImageDataか判断する
    static bool isType(const unsigned char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
