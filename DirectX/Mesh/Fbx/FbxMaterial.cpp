#include "FbxMaterial.h"
#include "../../System/AssetsManager.h"
#include "../../Utility/FileUtil.h"
#include <cassert>
#include <sstream>
#include <string>

FbxMaterial::FbxMaterial(
    const std::string& filePath,
    const FbxObject& objectsObject,
    const std::unordered_multimap<unsigned, unsigned>& connections,
    const std::unordered_map<unsigned, unsigned short>& lclModelNodeIDMap
) {
    //key: マテリアルノードID, value: マテリアルの添字
    std::unordered_map<unsigned, unsigned short> materialNodeMap;

    //ファイルパスからファイル名を取得しておく
    auto filename = FileUtil::getFileNameFromDirectry(filePath);
    filename = filename.substr(0, filename.length() - 4);

    mMaterials.resize(lclModelNodeIDMap.size());

    const auto& children = objectsObject.children;
    auto matRange = children.equal_range("Material");
    for (auto& r = matRange.first; r != matRange.second; ++r) {
        const auto& obj = r->second;
        unsigned nodeID = obj.getNodeId();
        auto range = connections.equal_range(nodeID);
        for (auto& r = range.first; r != range.second; ++r) {
            auto itr = lclModelNodeIDMap.find(r->second);
            if (itr == lclModelNodeIDMap.end()) {
                continue;
            }

            auto index = itr->second;
            parseMaterial(mMaterials[index], obj);
            mMaterials[index].name = filename + std::to_string(index);
            materialNodeMap.emplace(nodeID, index);
        }
    }

    auto texRange = children.equal_range("Texture");
    for (auto& r = texRange.first; r != texRange.second; ++r) {
        const auto& obj = r->second;
        unsigned nodeID = obj.getNodeId();
        auto range = connections.equal_range(nodeID);
        for (auto& r = range.first; r != range.second; ++r) {
            auto itr = materialNodeMap.find(r->second);
            if (itr == materialNodeMap.end()) {
                continue;
            }

            parseTexture(mMaterials[itr->second], filePath, obj);
        }
    }
}

FbxMaterial::~FbxMaterial() = default;

const Material& FbxMaterial::getMaterial(unsigned index) const {
    assert(index < getMaterialCount());
    return mMaterials[index];
}

unsigned FbxMaterial::getMaterialCount() const {
    return mMaterials.size();
}

void FbxMaterial::parseMaterial(
    Material& material,
    const FbxObject& materialObject
) const {
    //それぞれが絶対に読み込めるか確証がないため1つ1つ確認してから読み込む
    if (materialObject.hasProperties("Ambient")) {
        const auto& value = materialObject.getProperties("Ambient").value;
        std::istringstream iss(value);

        auto& a = material.ambient;
        iss >> a.x >> a.y >> a.z;
    }
    if (materialObject.hasProperties("Diffuse")) {
        const auto& value = materialObject.getProperties("Diffuse").value;
        std::istringstream iss(value);

        auto& d = material.diffuse;
        iss >> d.x >> d.y >> d.z;
    }
    if (materialObject.hasProperties("Emissive")) {
        const auto& value = materialObject.getProperties("Emissive").value;
        std::istringstream iss(value);

        auto& e = material.emissive;
        iss >> e.x >> e.y >> e.z;
    }
    if (materialObject.hasProperties("TransparencyFactor")) {
        const auto& value = materialObject.getProperties("TransparencyFactor").value;
        material.transparency = std::stof(value);
    }

    //ShadingModelがphongのときは追加で読み込む
    if (materialObject.getValue("ShadingModel") == "phong") {
        if (materialObject.hasProperties("Specular")) {
            const auto& value = materialObject.getProperties("Specular").value;
            std::istringstream iss(value);

            auto& s = material.specular;
            iss >> s.x >> s.y >> s.z;
        }
        if (materialObject.hasProperties("Shininess")) {
            const auto& value = materialObject.getProperties("Shininess").value;
            material.shininess = std::stof(value);
        }
    }
}

void FbxMaterial::parseTexture(
    Material& material,
    const std::string& filePath,
    const FbxObject& textureObject
) const {
    const auto& value = textureObject.getValue("Media");
    //7は"Video::"の文字数
    auto baseTextureName = value.substr(7);

    auto directryPath = FileUtil::getDirectryFromFilePath(filePath);
    material.textureID = AssetsManager::instance().createTexture(baseTextureName, directryPath);
}
