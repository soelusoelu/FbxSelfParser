#include "FbxTexture.h"

FbxTexture::FbxTexture()
    : mBaseTextureName()
{
}

FbxTexture::~FbxTexture() = default;

void FbxTexture::parse(std::ifstream& inFile) {
    std::string line;
    while (std::getline(inFile, line)) {
        auto foundPos = line.find(':');
        auto key = line.substr(0, foundPos);

        //タブを削除する
        while (key[0] == '\t') {
            key.erase(key.begin());
        }

        if (key == "Media") {
            mBaseTextureName = getString(line);

            //7はVideo::の文字数分
            mBaseTextureName = mBaseTextureName.substr(7);

            return;
        }
    }
}

const std::string& FbxTexture::getBaseTextureName() const {
    return mBaseTextureName;
}

bool FbxTexture::isUseBaseTexture() const {
    return (mBaseTextureName.length() > 0);
}

std::string FbxTexture::getString(const std::string& src) const {
    //文字列の部分を探し出す
    auto foundFirstPos = src.find('"') + 1;
    auto foundLastPos = src.find('"', foundFirstPos);

    //文字列の部分を分割する
    return src.substr(foundFirstPos, foundLastPos - foundFirstPos);
}
