#pragma once

#include "IAssetsRenderTextureDeleter.h"
#include "ICurrentSelectTextureGetter.h"
#include <memory>

class MeshRenderOnTexture;

class AssetsRenderTextureDeleter {
public:
    AssetsRenderTextureDeleter();
    ~AssetsRenderTextureDeleter();
    void initialize(IAssetsRenderTextureDeleter* deleter, const ICurrentSelectTextureGetter* getter);
    void update();

private:
    AssetsRenderTextureDeleter(const AssetsRenderTextureDeleter&) = delete;
    AssetsRenderTextureDeleter& operator=(const AssetsRenderTextureDeleter&) = delete;

private:
    IAssetsRenderTextureDeleter* mDeleter;
    const ICurrentSelectTextureGetter* mCurrnetTextureGetter;
};
