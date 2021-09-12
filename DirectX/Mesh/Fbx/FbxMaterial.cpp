#include "FbxMaterial.h"
#include <algorithm>
#include <sstream>

FbxMaterial::FbxMaterial()
    : mMaterial()
{
}

FbxMaterial::~FbxMaterial() = default;

void FbxMaterial::parse(std::ifstream& inFile) {
    std::string line;
    while (std::getline(inFile, line)) {
        auto foundPos = line.find(':');
        auto key = line.substr(0, foundPos);

        //タブを削除する
        while (key[0] == '\t') {
            key.erase(key.begin());
        }

        if (key == "Properties70") {
            parseMaterial(inFile);
            return;
        }
    }
}

const Material& FbxMaterial::getMaterial() const {
    return mMaterial;
}

void FbxMaterial::parseMaterial(std::ifstream& inFile) {
    std::string line;
    while (std::getline(inFile, line)) {
        if (line.find('}') != std::string::npos) {
            return;
        }

        auto foundPos = line.find("P:");
        line = line.substr(foundPos + 2);

        //カンマをスペースに置き換える
        std::replace(line.begin(), line.end(), ',', ' ');

        std::istringstream lineStream(line);

        std::string name, tmp;
        lineStream >> name >> tmp >> tmp >> tmp;
        if (name == "\"Ambient\"") {
            auto& a = mMaterial.ambient;
            lineStream >> a.x >> a.y >> a.z;
        } else if (name == "\"Diffuse\"") {
            auto& d = mMaterial.diffuse;
            lineStream >> d.x >> d.y >> d.z;
        } else if (name == "\"Specular\"") {
            auto& s = mMaterial.specular;
            lineStream >> s.x >> s.y >> s.z;
        } else if (name == "\"Shininess\"") {
            auto& s = mMaterial.shininess;
            lineStream >> s;
        } else if (name == "\"Emissive\"") {
            auto& e = mMaterial.emissive;
            lineStream >> e.x >> e.y >> e.z;
        }
    }
}
