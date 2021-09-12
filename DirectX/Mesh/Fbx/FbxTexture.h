#pragma once

#include <fstream>
#include <string>

class FbxTexture {
public:
    FbxTexture();
    ~FbxTexture();
    FbxTexture(const FbxTexture&) = delete;
    FbxTexture& operator=(const FbxTexture&) = delete;

    void parse(std::ifstream& inFile);
    const std::string& getBaseTextureName() const;
    bool isUseBaseTexture() const;

private:
    //ダブルクォーテーションで囲まれた文字列を取得する
    std::string getString(const std::string& src) const;

private:
    std::string mBaseTextureName;
};
