#include "FbxMaterial.h"
#include <sstream>

FbxMaterial::FbxMaterial(const FbxObject& objectsObject)
    : mObjectsObject(objectsObject)
    , mMaterial()
    , mBaseTextureName()
{
    parse();
}

FbxMaterial::~FbxMaterial() = default;

const Material& FbxMaterial::getMaterial() const {
    return mMaterial;
}

const std::string& FbxMaterial::getBaseTextureName() const {
    return mBaseTextureName;
}

void FbxMaterial::parse() {
    if (mObjectsObject.hasObject("Material")) {
        const auto& materialObject = mObjectsObject.getObject("Material");
        parseMaterial(materialObject);
    }
    if (mObjectsObject.hasObject("Texture")) {
        const auto& textureObject = mObjectsObject.getObject("Texture");
        parseTexture(textureObject);
    }
}

void FbxMaterial::parseMaterial(const FbxObject& materialObject) {
    if (materialObject.hasProperties("Ambient")) {
        const auto& value = materialObject.getProperties("Ambient").value;
        std::istringstream iss(value);

        auto& a = mMaterial.ambient;
        iss >> a.x >> a.y >> a.z;
    }
    if (materialObject.hasProperties("Diffuse")) {
        const auto& value = materialObject.getProperties("Diffuse").value;
        std::istringstream iss(value);

        auto& d = mMaterial.diffuse;
        iss >> d.x >> d.y >> d.z;
    }
    if (materialObject.hasProperties("Specular")) {
        const auto& value = materialObject.getProperties("Specular").value;
        std::istringstream iss(value);

        auto& s = mMaterial.specular;
        iss >> s.x >> s.y >> s.z;
    }
    if (materialObject.hasProperties("Emissive")) {
        const auto& value = materialObject.getProperties("Emissive").value;
        std::istringstream iss(value);

        auto& e = mMaterial.emissive;
        iss >> e.x >> e.y >> e.z;
    }
    if (materialObject.hasProperties("Opacity")) {
        const auto& value = materialObject.getProperties("Opacity").value;
        mMaterial.transparency = std::stof(value);
    }
    if (materialObject.hasProperties("Shininess")) {
        const auto& value = materialObject.getProperties("Shininess").value;
        mMaterial.shininess = std::stof(value);
    }
}

void FbxMaterial::parseTexture(const FbxObject& textureObject) {
    const auto& value = textureObject.getValue("Media");
    //7は"Video::"の文字数
    mBaseTextureName = value.substr(7);
}
