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

    void parse(Material& material, const std::string& filePath) const;

private:
    //マテリアルを読み込む
    void parseMaterial(Material& material, const FbxObject& materialObject) const;
    //テクスチャを読み込む
    void parseTexture(Material& material, const std::string& filePath, const FbxObject& textureObject) const;

private:
    const FbxObject& mObjectsObject;
};
