#include "MeshCommonShaderSetter.h"
#include "../System/AssetsManager.h"
#include "../System/Texture/TextureBinder.h"

void MeshCommonShaderSetter::setCommon(
    MeshCommonConstantBuffer& out,
    const Matrix4& world,
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) {
    out.world = world;
    out.view = view;
    out.projection = projection;
    out.wvp = world * view * projection;
    out.lightDir = dirLightDirection;
    out.lightColor = dirLightColor;
    out.cameraPos = cameraPosition;
}

void MeshCommonShaderSetter::setMaterial(
    MaterialConstantBuffer& out,
    const Material& material,
    bool transferTexture
) {
    out.ambient = material.ambient;
    out.diffuse = Vector4(material.diffuse, material.transparency);
    out.specular = material.specular;
    out.shininess = material.shininess;

    //テクスチャ登録
    if (transferTexture) {
        TextureBinder::bind(material.textureID);
    }
}
