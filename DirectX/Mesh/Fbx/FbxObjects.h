#pragma once

#include <fstream>
#include <memory>

class FbxMesh;
class FbxMaterial;
class FbxTexture;

class FbxObjects {
public:
    FbxObjects();
    ~FbxObjects();
    FbxObjects(const FbxObjects&) = delete;
    FbxObjects& operator=(const FbxObjects&) = delete;

    void parse(std::ifstream& inFile);
    const FbxMesh& getMesh() const;
    const FbxMaterial& getMaterial() const;
    const FbxTexture& getTexture() const;

private:
    std::unique_ptr<FbxMesh> mMeshParser;
    std::unique_ptr<FbxMaterial> mMaterialParser;
    std::unique_ptr<FbxTexture> mTextureParser;
};
