#pragma once

#include "Reader/FbxObject.h"
#include "../Material.h"
#include <string>
#include <unordered_map>
#include <vector>

class FbxMaterial {
public:
    FbxMaterial(
        const FbxObject& objectsObject,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxMaterial();
    FbxMaterial(const FbxMaterial&) = delete;
    FbxMaterial& operator=(const FbxMaterial&) = delete;

    void parse(
        std::vector<Material>& materials,
        const std::string& filePath,
        const std::unordered_map<unsigned, unsigned short>& lclModelNodeIDMap
    );

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
    const FbxObject& mObjectsObject;
    const std::unordered_multimap<unsigned, unsigned>& mConnections;
    //key: ノードID, value: マテリアルの添字
    std::unordered_map<unsigned, unsigned> mMaterialMap;
};
