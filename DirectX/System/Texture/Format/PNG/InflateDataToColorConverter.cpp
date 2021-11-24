#include "InflateDataToColorConverter.h"

InflateDataToColorConverter::InflateDataToColorConverter() = default;

InflateDataToColorConverter::~InflateDataToColorConverter() = default;

void InflateDataToColorConverter::convert(
    std::vector<unsigned char>& out,
    const std::vector<unsigned char>& inData,
    const ImageHeaderChunk& header
) const {
    auto w = header.width;
    auto h = header.height;
    out.resize(w * h * 4);

    if (header.getColorType() == ImageHeaderChunk::ColorType::TRUE_COLOR_ALPHA) {
        convertColorAlpha(out, inData, w, h);
    } else {
        assert(false);
    }
}

void InflateDataToColorConverter::convertColorAlpha(
    std::vector<unsigned char>& out,
    const std::vector<unsigned char>& inData,
    unsigned width,
    unsigned height
) const {
    //行のバイトサイズ
    int rowByteSize = width * 4;
    //解凍したデータの行の長さ
    int rowLength = rowByteSize + 1;

    for (unsigned y = 0; y < height; ++y) {
        //現在の行の先頭
        auto rowHead = rowLength * y;
        //行の先頭は色の持ち方
        auto filterType = inData[rowHead];

        for (unsigned x = 0; x < width; ++x) {
            int inIdx = rowHead + x * 4 + 1;
            int outIdx = y * rowByteSize + x * 4;

            if (filterType == 0) { //0はNone
                convertColor(out, outIdx, inData, inIdx);
            } else if (filterType == 1) { //1はSub
                if (x > 0) {
                    //左隣のピクセルの色との差分
                    convertColorAddition(out, outIdx, inData, inIdx, out, outIdx - 4);
                } else {
                    //xが0のときは左隣のピクセルがないのでそのまま代入
                    convertColor(out, outIdx, inData, inIdx);
                }
            } else if (filterType == 2) { //2はUp
                if (y > 0) {
                    //真上のピクセルの色との差分
                    convertColorAddition(out, outIdx, inData, inIdx, out, outIdx - rowByteSize);
                } else {
                    //yが0のときは真上のピクセルがないのでそのまま代入
                    convertColor(out, outIdx, inData, inIdx);
                }
            } else if (filterType == 3) { //3はAverage
                if (x > 0 && y > 0) {
                    convertColorAverage(out, outIdx, inData, inIdx, out, outIdx - 4, outIdx - rowByteSize);
                } else {
                    //xもしくはyが0のときはそのまま代入
                    convertColor(out, outIdx, inData, inIdx);
                }
            } else if (filterType == 4) { //4はPaeth
                if (x > 0 && y > 0) {
                    convertColorPaeth(out, outIdx, inData, inIdx, out, outIdx - 4, outIdx - rowByteSize, outIdx - rowByteSize - 4);
                } else {
                    //xもしくはyが0のときはそのまま代入
                    convertColor(out, outIdx, inData, inIdx);
                }
            } else {
                assert(false);
            }
        }
    }
}

void InflateDataToColorConverter::convertColor(
    std::vector<unsigned char>& out,
    unsigned outStartIndex,
    const std::vector<unsigned char>& inData,
    unsigned inStartIndex
) const {
    out[outStartIndex] = inData[inStartIndex];
    out[outStartIndex + 1] = inData[inStartIndex + 1];
    out[outStartIndex + 2] = inData[inStartIndex + 2];
    out[outStartIndex + 3] = inData[inStartIndex + 3];
}

void InflateDataToColorConverter::convertColorAddition(
    std::vector<unsigned char>& out,
    unsigned outStartIndex,
    const std::vector<unsigned char>& inData1,
    unsigned inStartIndex1,
    const std::vector<unsigned char>& inData2,
    unsigned inStartIndex2
) const {
    out[outStartIndex] = inData1[inStartIndex1] + inData2[inStartIndex2];
    out[outStartIndex + 1] = inData1[inStartIndex1 + 1] + inData2[inStartIndex2 + 1];
    out[outStartIndex + 2] = inData1[inStartIndex1 + 2] + inData2[inStartIndex2 + 2];
    out[outStartIndex + 3] = inData1[inStartIndex1 + 3] + inData2[inStartIndex2 + 3];
}

void InflateDataToColorConverter::convertColorAverage(
    std::vector<unsigned char>& out,
    unsigned outStartIndex,
    const std::vector<unsigned char>& inData1,
    unsigned inStartIndex1,
    const std::vector<unsigned char>& inData2,
    unsigned inStartIndex2_1,
    unsigned inStartIndex2_2
) const {
    unsigned char avgR = (inData2[inStartIndex2_1] + inData2[inStartIndex2_2]) / 2;
    unsigned char avgG = (inData2[inStartIndex2_1 + 1] + inData2[inStartIndex2_2 + 1]) / 2;
    unsigned char avgB = (inData2[inStartIndex2_1 + 2] + inData2[inStartIndex2_2 + 2]) / 2;
    unsigned char avgA = (inData2[inStartIndex2_1 + 3] + inData2[inStartIndex2_2 + 3]) / 2;

    out[outStartIndex] = inData1[inStartIndex1] + avgR;
    out[outStartIndex + 1] = inData1[inStartIndex1 + 1] + avgG;
    out[outStartIndex + 2] = inData1[inStartIndex1 + 2] + avgB;
    out[outStartIndex + 3] = inData1[inStartIndex1 + 3] + avgA;
}

void InflateDataToColorConverter::convertColorPaeth(
    std::vector<unsigned char>& out,
    unsigned outStartIndex,
    const std::vector<unsigned char>& inData1,
    unsigned inStartIndex1,
    const std::vector<unsigned char>& inData2,
    unsigned inStartIndex2_1,
    unsigned inStartIndex2_2,
    unsigned inStartIndex2_3
) const {
    float r = paethPredictor(inData2[inStartIndex2_1], inData2[inStartIndex2_2], inData2[inStartIndex2_3]);
    float g = paethPredictor(inData2[inStartIndex2_1 + 1], inData2[inStartIndex2_2 + 1], inData2[inStartIndex2_3] + 1);
    float b = paethPredictor(inData2[inStartIndex2_1 + 2], inData2[inStartIndex2_2 + 2], inData2[inStartIndex2_3] + 2);
    float a = paethPredictor(inData2[inStartIndex2_1 + 3], inData2[inStartIndex2_2 + 3], inData2[inStartIndex2_3] + 3);

    out[outStartIndex] = inData1[inStartIndex1] + r;
    out[outStartIndex + 1] = inData1[inStartIndex1 + 1] + g;
    out[outStartIndex + 2] = inData1[inStartIndex1 + 2] + b;
    out[outStartIndex + 3] = inData1[inStartIndex1 + 3] + a;
}

float InflateDataToColorConverter::paethPredictor(float a, float b, float c) const {
    float p = a + b - c;
    float pa = fabsf(p - a);
    float pb = fabsf(p - b);
    float pc = fabsf(p - c);
    if (pa <= pb && pa <= pc) {
        return a;
    } else if (pb <= pc) {
        return b;
    } else {
        return c;
    }
}
