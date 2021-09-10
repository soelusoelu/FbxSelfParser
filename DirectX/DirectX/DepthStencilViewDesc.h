#pragma once

#include "Format.h"

enum class DSVDimension {
    TEXTURE2D
};

enum class DSVFlag {
    READ_ONLY_DEPTH,
    READ_ONLY_STENCIL
};

//深度ステンシルビューからアクセスできるテクスチャのサブリソース
struct DepthStencilViewDesc {
    Format format;
    DSVDimension viewDimension;
    //テクスチャが読み取り専用かの値
    //0で読み取り専用、それ以外の場合はDSVFlagのメンバを渡す
    unsigned flags;
    //Texture2Dのミップスライス
    unsigned mipSlice;

    DepthStencilViewDesc()
        : format(Format::FORMAT_D24_UNORM_S8_UINT)
        , viewDimension(DSVDimension::TEXTURE2D)
        , flags(0)
        , mipSlice(0) {
    }
};