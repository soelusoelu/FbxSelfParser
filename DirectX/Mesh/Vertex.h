#pragma once

#include "../Math/Math.h"

//最大ウェイト数
constexpr int MAX_NUM_WEIGHT = 4;

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;

    //スキンメッシュ用
    float weight[MAX_NUM_WEIGHT];
    unsigned short index[MAX_NUM_WEIGHT];

    MeshVertex()
        : pos(Vector3::zero)
        , normal(Vector3::zero)
        , uv(Vector2::zero)
        , weight{ 0.f, 0.f, 0.f, 0.f }
        , index{ 0, 0, 0, 0 }
    {
    }
};

struct PosNormUVVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};
