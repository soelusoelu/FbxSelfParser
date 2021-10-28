#pragma once

#include "Reader/FbxObject.h"
#include "../Material.h"
#include <string>
#include <unordered_map>
#include <vector>

class FbxMaterial {
public:
    FbxMaterial(
        const std::string& filePath,
        const FbxObject& objectsObject,
        const std::unordered_multimap<unsigned, unsigned>& connections,
        const std::unordered_map<unsigned, unsigned short>& lclModelNodeIDMap
    );
    ~FbxMaterial();
    FbxMaterial(const FbxMaterial&) = delete;
    FbxMaterial& operator=(const FbxMaterial&) = delete;

    //マテリアル配列を取得する
    const Material& getMaterial(unsigned index) const;
    //マテリアル数を取得する
    unsigned getMaterialCount() const;

private:
    //マテリアルを読み込む
    void parseMaterial(
        Material& material,
        const FbxObject& materialObject
    ) const;

    //テクスチャを読み込む
    void parseTexture(
        Material& material,
        const std::string& filePath,
        const FbxObject& textureObject
    ) const;

private:
    //メッシュ順のマテリアル
    std::vector<Material> mMaterials;
};
