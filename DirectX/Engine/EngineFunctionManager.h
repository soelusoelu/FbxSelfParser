#pragma once

#include "IEngineManagingClassGetter.h"
#include "../Device/FileOperator.h"
#include "../Math/Math.h"
#include <memory>
#include <string>

class Camera;
class Renderer;
class ModelViewer;

//エンジン機能統括クラス
class EngineFunctionManager
    : public FileOperator
    , public IEngineManagingClassGetter
{
public:
    EngineFunctionManager();
    ~EngineFunctionManager();

    virtual DebugManager& debug() const override;
    virtual AssetsRenderTextureManager& getAssetsRenderTextureManager() const override;

    //初期化
    void initialize();

    //アップデート前処理
    void preUpdateProcess();
    //毎フレーム更新
    void update();

    //2D関連の描画
    void draw2D(const Renderer& renderer, Matrix4& proj) const;
    //2Dデバッグ関連の描画
    void drawDebug2D(Matrix4& proj) const;

    //3D関連の描画
    void draw3D(const Renderer& renderer) const;

private:
    EngineFunctionManager(const EngineFunctionManager&) = delete;
    EngineFunctionManager& operator=(const EngineFunctionManager&) = delete;

    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::unique_ptr<DebugManager> mDebugManager;
    std::unique_ptr<AssetsRenderTextureManager> mAssetsRenderTextureManager;
    std::unique_ptr<ModelViewer> mModelViewer;
};
