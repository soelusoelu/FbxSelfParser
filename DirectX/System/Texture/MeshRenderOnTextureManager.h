#pragma once

#include "../../Math/Math.h"
#include <memory>
#include <list>

class MeshRenderOnTextureComponent;

class MeshRenderOnTextureManager {
    using MeshRenderOnTexturePtr = std::shared_ptr<MeshRenderOnTextureComponent>;
    using MeshRenderOnTexturePtrList = std::list<MeshRenderOnTexturePtr>;

public:
    MeshRenderOnTextureManager();
    ~MeshRenderOnTextureManager();
    //毎フレーム更新
    void update();
    //テクスチャを追加する
    void add(const MeshRenderOnTexturePtr& texture);
    //メッシュをテクスチャ上に描画する
    void drawMeshOnTextures() const;
    //メッシュ描画済みテクスチャを描画する
    void drawTextures(const Matrix4& proj) const;
    //保持してる全テクスチャを削除する
    void clear();
    //指定のテクスチャを削除する
    void erase(const MeshRenderOnTexturePtr& texture);

private:
    MeshRenderOnTextureManager(const MeshRenderOnTextureManager&) = delete;
    MeshRenderOnTextureManager& operator=(const MeshRenderOnTextureManager&) = delete;

    //不要なテクスチャを削除する
    void removeDestroyTexture();

private:
    MeshRenderOnTexturePtrList mTextures;
};
