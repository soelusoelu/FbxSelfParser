#pragma once

#include "IAddAssets.h"
#include "IAssetsRenderTextureDeleter.h"
#include "IAssetsRenderTexturesGetter.h"
#include "../../Device/FileOperator.h"
#include "../../Math/Math.h"
#include <list>
#include <memory>
#include <string>
#include <unordered_set>

//メッシュを描画したテクスチャ管理クラス
class AssetsRenderTextureList
    : public FileOperator
    , public IAddAssets
    , public IAssetsRenderTextureDeleter
    , public IAssetsRenderTexturesGetter
{
public:
    AssetsRenderTextureList();
    ~AssetsRenderTextureList();
    virtual void add(const std::string& filePath) override;
    virtual void add(const std::string& fileName, const std::string& directoryPath) override;
    virtual void deleteTexture(const std::string& filePath) override;
    virtual const AssetsRenderTexturePtrArray& getTextures() const override;

    void initialize();
    //メッシュをテクスチャに描画する
    void drawMeshOnTexture();
    //メッシュ描画済みテクスチャを描画する
    void drawTexture(const Matrix4& proj) const;

private:
    AssetsRenderTextureList(const AssetsRenderTextureList&) = delete;
    AssetsRenderTextureList& operator=(const AssetsRenderTextureList&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //テクスチャを作成し格納する
    void createTexture(const std::string& fileName, const std::string& directoryPath);
    void createTexture(const std::string& filePath);
    //テクスチャを並び替える
    void rearrangeTextures();
    //テクスチャ位置を設定する
    void setTexturePosition(AssetsRenderTexture& target, int textureNo);
    //ファイルパスが読み込み済みか
    bool loadedFilePath(const std::string& filePath) const;

private:
    AssetsRenderTexturePtrArray mTextures;
    AssetsRenderTexturePtrArray mNonDrawTextures;
    std::unordered_set<std::string> mTexturesFilePath;

    //縦横共通サイズ
    int mTextureSize;
    //テクスチャの最大列表示数
    int mColumnDisplayLimit;
    //テクスチャの標示間隔
    int mTextureDisplayInterval;
};
