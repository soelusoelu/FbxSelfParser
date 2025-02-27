﻿#pragma once

#include "../Math/Math.h"
#include <string>

struct Material {
    Vector3 ambient; //アンビエント
    Vector3 diffuse; //ディフューズ
    Vector3 specular; //スペキュラー
    Vector3 emissive;
    Vector3 bump;
    float transparency; //透明値
    float shininess;
    int textureID; //テクスチャがある場合のテクスチャID
    std::string name; //マテリアル名

    static constexpr int INVALID_ID = -1;

    Material()
        : ambient(Vector3::zero)
        , diffuse(Vector3::one)
        , specular(Vector3::zero)
        , emissive(Vector3::zero)
        , bump(Vector3::zero)
        , transparency(1.f)
        , shininess(1.f)
        , textureID(INVALID_ID)
        , name("default")
    {
    }
};
