#include "FbxObjects.h"
#include "FbxMaterial.h"
#include "FbxMesh.h"
#include "FbxTexture.h"
#include <sstream>

FbxObjects::FbxObjects()
    : mMeshParser(std::make_unique<FbxMesh>())
    , mMaterialParser(std::make_unique<FbxMaterial>())
    , mTextureParser(std::make_unique<FbxTexture>())
{
}

FbxObjects::~FbxObjects() = default;

void FbxObjects::parse(std::ifstream& inFile) {
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream lineStream(line);

        //コロン区切りで行の先頭文字列を取得
        std::string key;
        std::getline(lineStream, key, ':');

        //タブを削除する
        while (key[0] == '\t') {
            key.erase(key.begin());
        }

        if (key == "Geometry") {
            mMeshParser->parse(inFile);
        } else if (key == "Material") {
            mMaterialParser->parse(inFile);
        } else if (key == "Texture") {
            mTextureParser->parse(inFile);
        }
    }
}

const FbxMesh& FbxObjects::getMesh() const {
    return *mMeshParser;
}

const FbxMaterial& FbxObjects::getMaterial() const {
    return *mMaterialParser;
}

const FbxTexture& FbxObjects::getTexture() const {
    return *mTextureParser;
}
