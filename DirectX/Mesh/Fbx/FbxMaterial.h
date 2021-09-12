#pragma once

#include "../Material.h"
#include <fstream>

class FbxMaterial {
public:
    FbxMaterial();
    ~FbxMaterial();
    FbxMaterial(const FbxMaterial&) = delete;
    FbxMaterial& operator=(const FbxMaterial&) = delete;

    void parse(std::ifstream& inFile);
    const Material& getMaterial() const;

private:
    //マテリアルを読み込む
    void parseMaterial(std::ifstream& inFile);

private:
    Material mMaterial;
};
