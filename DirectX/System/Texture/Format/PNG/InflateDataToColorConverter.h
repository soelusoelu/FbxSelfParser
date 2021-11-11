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

    //入力データから色を決定する
    void convertColor(
        std::vector<unsigned char>& out,
        unsigned outStartIndex,
        const std::vector<unsigned char>& inData,
        unsigned inStartIndex
    ) const;
    //2つの入力データの合計から色を決定する
    void convertColorAddition(
        std::vector<unsigned char>& out,
        unsigned outStartIndex,
        const std::vector<unsigned char>& inData1,
        unsigned inStartIndex1,
        const std::vector<unsigned char>& inData2,
        unsigned inStartIndex2
    ) const;
    //2つの入力データの平均値から色を決定する
    void convertColorAverage(
        std::vector<unsigned char>& out,
        unsigned outStartIndex,
        const std::vector<unsigned char>& inData1,
        unsigned inStartIndex1,
        const std::vector<unsigned char>& inData2,
        unsigned inStartIndex2_1,
        unsigned inStartIndex2_2
    ) const;
    //謎の手法
    void convertColorPaeth(
        std::vector<unsigned char>& out,
        unsigned outStartIndex,
        const std::vector<unsigned char>& inData1,
        unsigned inStartIndex1,
        const std::vector<unsigned char>& inData2,
        unsigned inStartIndex2_1,
        unsigned inStartIndex2_2,
        unsigned inStartIndex2_3
    ) const;
    float paethPredictor(
        float a,
        float b,
        float c
    ) const;
};
