#pragma once

#include "IAddAssets.h"
#include "../../Device/FileOperator.h"
#include "../../Math/Math.h"
#include <memory>
#include <string>

class SpriteButton;

//メッシュを描画したテクスチャ追加クラス
class AssetsRenderTextureAdder
    : public FileOperator
{
public:
    AssetsRenderTextureAdder();
    ~AssetsRenderTextureAdder();
    void initialize(IAddAssets* adder);
    void update();

private:
    AssetsRenderTextureAdder(const AssetsRenderTextureAdder&) = delete;
    AssetsRenderTextureAdder& operator=(const AssetsRenderTextureAdder&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    void onClickButton();
    void onChangeMode();

private:
    std::unique_ptr<SpriteButton> mButton;
    IAddAssets* mAssetsAdder;
    std::string mSpriteFilePath;
    Vector2 mRenderPosition;
};
