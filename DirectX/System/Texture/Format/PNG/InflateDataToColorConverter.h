#pragma once

#include "ImageHeaderChunk.h"
#include <vector>

class InflateDataToColorConverter {
public:
    InflateDataToColorConverter();
    ~InflateDataToColorConverter();
    InflateDataToColorConverter(const InflateDataToColorConverter&) = delete;
    InflateDataToColorConverter& operator=(const InflateDataToColorConverter&) = delete;

    //解凍したデータをRGBAの配列に変換する
    void convert(
        std::vector<unsigned char>& out,
        const std::vector<unsigned char>& inData,
        const ImageHeaderChunk& header
    ) const;

private:
    //RGBAの変換
    void convertColorAlpha(
        std::vector<unsigned char>& out,
        const std::vector<unsigned char>& inData,
        unsigned width,
        unsigned height
    ) const;

    void convertColor(
        std::vector<unsigned char>& out,
        const std::vector<unsigned char>& inData,
        unsigned outStartIndex,
        unsigned inStartIndex
    ) const;
    void convertColor(
        std::vector<unsigned char>& out,
        unsigned char r,
        unsigned char g,
        unsigned char b,
        unsigned char a,
        unsigned outStartIndex
    ) const;
};
