#pragma once

//頂点タイプ
enum class VertexType {
    FLOAT1,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    BYTE4,
    BYTE4N,
    UBYTE4,
    UBYTE4N,
    SHORT2,
    SHORT2N,
    USHORT2,
    USHORT2N,
    SHORT4,
    SHORT4N,
    USHORT4,
    USHORT4N,
    INT1,
    INT2,
    INT3,
    INT4,
    UINT1,
    UINT2,
    UINT3,
    UINT4
};

//頂点か否か
enum class SlotClass {
    VERTEX_DATA,
    INSTANCE_DATA
};

//頂点宣言ディスクリプタ
struct InputElementDesc {
    //セマンティック LPCSTR
    const char* semantic;
    //セマンティックインデックス
    unsigned index;
    //頂点タイプ
    VertexType type;
    //スロット
    unsigned slot;
    //オフセット
    unsigned offset;
    //頂点か否か
    SlotClass classification;
    //ストリーム番号
    unsigned stream;
};

//頂点宣言の最大値
constexpr unsigned VERTEX_DECLARATION_MAX = 16;
