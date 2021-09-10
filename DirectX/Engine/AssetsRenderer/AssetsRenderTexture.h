#pragma once

#include "../../Math/Math.h"
#include <memory>
#include <string>

class MeshRenderOnTexture;

//アセットを描画したテクスチャ
class AssetsRenderTexture {
public:
    AssetsRenderTexture(const std::string& filePath, int size);
    ~AssetsRenderTexture();
    //メッシュをテクスチャ上に描画する
    void drawMesh() const;
    //MeshRenderOnTextureを取得する
    MeshRenderOnTexture& getTexture() const;

private:
    AssetsRenderTexture(const AssetsRenderTexture&) = delete;
    AssetsRenderTexture& operator=(const AssetsRenderTexture&) = delete;

private:
    std::unique_ptr<MeshRenderOnTexture> mTexture;

    static constexpr inline float FOV = 45.f;
    static constexpr inline float NEAR_CLIP = 0.1f;
    static constexpr inline float FAR_CLIP = 1000.f;
    static const inline Vector3 DIRECTION = Vector3(1.f, -1.f, 1.f);
};
