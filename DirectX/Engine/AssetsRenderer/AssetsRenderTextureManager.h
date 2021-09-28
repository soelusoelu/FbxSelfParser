#pragma once

#include "ICallbackSelectAssetsTexture.h"
#include "ICurrentSelectTextureGetter.h"
#include "../../Math/Math.h"
#include "../../System/Json/JsonObject.h"
#include "../../Utility/FileMode.h"
#include <memory>

class AssetsRenderTextureList;
class AssetsRenderTextureAdder;
class AssetsRenderTextureDeleter;
class AssetsTexturesSelector;
class AssetsRenderTexture;

//メッシュ描画テクスチャ関連総管理クラス
class AssetsRenderTextureManager
    : public ICurrentSelectTextureGetter
{
public:
    AssetsRenderTextureManager();
    ~AssetsRenderTextureManager();
    AssetsRenderTextureManager(const AssetsRenderTextureManager&) = delete;
    AssetsRenderTextureManager& operator=(const AssetsRenderTextureManager&) = delete;

    virtual const AssetsRenderTexture& getCurrentSelectTexture() const override;
    virtual bool selectedTexture() const override;

    void saveAndLoad(JsonObject& inObj, FileMode mode);
    //初期化
    void initialize();
    //毎フレーム更新
    void update();
    //メッシュを描画する
    void drawMeshes() const;
    //テクスチャを描画する
    void drawTextures(const Matrix4& proj) const;
    //アセットテクスチャ選択コールバックを取得する
    ICallbackSelectAssetsTexture* getCallbackSelectAssetsTexture() const;

private:
    std::unique_ptr<AssetsRenderTextureList> mTextureList;
    std::unique_ptr<AssetsRenderTextureAdder> mTextureAdder;
    std::unique_ptr<AssetsRenderTextureDeleter> mDeleter;
    std::unique_ptr<AssetsTexturesSelector> mSelector;
    std::shared_ptr<AssetsRenderTexture> mCurrentSelectTexture;
};
