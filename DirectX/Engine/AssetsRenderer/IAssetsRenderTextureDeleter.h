#pragma once

#include <string>

class IAssetsRenderTextureDeleter {
public:
    virtual ~IAssetsRenderTextureDeleter() = default;
    virtual void deleteTexture(const std::string& filePath) = 0;
};
