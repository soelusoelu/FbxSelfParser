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
            int idx = rowHead + x * 4 + 1;
            int i = y * rowByteSize + x * 4;

            if (filterType == 0) { //0はNone
                convertColor(out, inData, i, idx);
            } else if (filterType == 1) { //1はSub
                if (x > 0) {
                    //左隣のピクセルの色との差分
                    unsigned r = inData[idx] + out[i - 4];
                    unsigned g = inData[idx + 1] + out[i - 3];
                    unsigned b = inData[idx + 2] + out[i - 2];
                    unsigned a = inData[idx + 3] + out[i - 1];

                    convertColor(out, r, g, b, a, i);
                } else {
                    //xが0のときは左隣のピクセルがないのでそのまま代入
                    convertColor(out, inData, i, idx);
                }
            } else {
                int a = 0;
            }
        }
    }
}

void InflateDataToColorConverter::convertColor(
    std::vector<unsigned char>& out,
    const std::vector<unsigned char>& inData,
    unsigned outStartIndex,
    unsigned inStartIndex
) const {
    out[outStartIndex] = inData[inStartIndex];
    out[outStartIndex + 1] = inData[inStartIndex + 1];
    out[outStartIndex + 2] = inData[inStartIndex + 2];
    out[outStartIndex + 3] = inData[inStartIndex + 3];
}

void InflateDataToColorConverter::convertColor(
    std::vector<unsigned char>& out,
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a,
    unsigned outStartIndex
) const {
    out[outStartIndex] = r;
    out[outStartIndex + 1] = g;
    out[outStartIndex + 2] = b;
    out[outStartIndex + 3] = a;
}
