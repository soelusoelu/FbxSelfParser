#include "FbxMaterial.h"
#include "../../System/AssetsManager.h"
#include "../../Utility/FileUtil.h"
#include <sstream>
#include <string>

FbxMaterial::FbxMaterial(
    const FbxObject& objectsObject,
    const std::multimap<unsigned, unsigned>& connections
)
    : mObjectsObject(objectsObject)
    , mConnections(connections)
{
}

FbxMaterial::~FbxMaterial() = default;

void FbxMaterial::parse(
    std::vector<Material>& materials,
    const std::string& filePath,
    const std::unordered_map<unsigned, unsigned>& lclModelNodeIDMap
) {
    const auto& children = mObjectsObject.children;
    for (const auto& child : children) {
        if (child.name == "Material") {
            unsigned nodeID = static_cast<unsigned>(std::stoi(child.attributes[0]));
            auto range = mConnections.equal_range(nodeID);
            for (auto& r = range.first; r != range.second; ++r) {
                auto itr = lclModelNodeIDMap.find(r->second);
                if (itr == lclModelNodeIDMap.end()) {
                    continue;
                }

                auto index = itr->second;
                parseMaterial(materials[index], child);
                mMaterialMap.emplace(nodeID, index);
            }
        } else if (child.name == "Texture") {
            unsigned nodeID = static_cast<unsigned>(std::stoi(child.attributes[0]));
            auto range = mConnections.equal_range(nodeID);
            for (auto& r = range.first; r != range.second; ++r) {
                auto itr = mMaterialMap.find(r->second);
                if (itr == mMaterialMap.end()) {
                    continue;
                }

                parseTexture(materials[itr->second], filePath, child);
            }
        }
    }
}

void FbxMaterial::parseMaterial(
    Material& material,
    const FbxObject& materialObject
) const {
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
    if (materialObject.hasProperties("Specular")) {
        const auto& value = materialObject.getProperties("Specular").value;
        std::istringstream iss(value);

        auto& s = material.specular;
        iss >> s.x >> s.y >> s.z;
    }
    if (materialObject.hasProperties("Emissive")) {
        const auto& value = materialObject.getProperties("Emissive").value;
        std::istringstream iss(value);

        auto& e = material.emissive;
        iss >> e.x >> e.y >> e.z;
    }
    if (materialObject.hasProperties("Opacity")) {
        const auto& value = materialObject.getProperties("Opacity").value;
        material.transparency = std::stof(value);
    }
    if (materialObject.hasProperties("Shininess")) {
        const auto& value = materialObject.getProperties("Shininess").value;
        material.shininess = std::stof(value);
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
