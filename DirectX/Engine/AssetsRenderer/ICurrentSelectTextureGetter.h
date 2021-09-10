#pragma once

class AssetsRenderTexture;

class ICurrentSelectTextureGetter {
public:
    virtual ~ICurrentSelectTextureGetter() = default;
    virtual const AssetsRenderTexture& getCurrentSelectTexture() const = 0;
    virtual bool selectedTexture() const = 0;
};
