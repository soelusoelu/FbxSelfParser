#pragma once

#include "Material.h"
#include "../Math/Math.h"
#include "../System/Shader/ConstantBuffers.h"

//メッシュ共通定数バッファを設定するクラス
class MeshCommonShaderSetter {
public:
    static void setCommon(
        MeshCommonConstantBuffer& out,
        const Matrix4& world,
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    );

    static void setMaterial(
        MaterialConstantBuffer& out,
        const Material& material,
        bool transferTexture = true
    );

private:
    MeshCommonShaderSetter() = delete;
    ~MeshCommonShaderSetter() = delete;
};
