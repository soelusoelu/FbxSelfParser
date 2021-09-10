#pragma once

#include <memory>
#include <vector>

class AssetsRenderTexture;

using AssetsRenderTexturePtr = std::shared_ptr<AssetsRenderTexture>;
using AssetsRenderTexturePtrArray = std::vector<AssetsRenderTexturePtr>;

class IAssetsRenderTexturesGetter {
public:
    virtual ~IAssetsRenderTexturesGetter() = default;
    virtual const AssetsRenderTexturePtrArray& getTextures() const = 0;
};