#pragma once

#include "Reader/FbxObject.h"
#include "../Material.h"
#include <string>

class FbxMaterial {
public:
    FbxMaterial(const FbxObject& objectsObject);
    ~FbxMaterial();
    FbxMaterial(const FbxMaterial&) = delete;
    FbxMaterial& operator=(const FbxMaterial&) = delete;

    const Material& getMaterial() const;
    const std::string& getBaseTextureName() const;

private:
    void parse();
    //マテリアルを読み込む
    void parseMaterial(const FbxObject& materialObject);
    //テクスチャを読み込む
    void parseTexture(const FbxObject& textureObject);

private:
    const FbxObject& mObjectsObject;
    Material mMaterial;
    std::string mBaseTextureName;
};
