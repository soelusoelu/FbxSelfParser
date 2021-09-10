#pragma once

#include "../../Device/FileOperator.h"
#include "../../Math/Math.h"
#include <memory>

class Camera;
class DirectionalLight;
class Renderer;
class DrawString;

class DebugManager
    : public FileOperator
{
public:
    DebugManager();
    ~DebugManager();

    //初期化
    void initialize();
    //アップデート前処理
    void preUpdateProcess();

    //2D関連の描画
    void draw2D(const Renderer& renderer, Matrix4& proj) const;
    //2Dデバッグ関連の描画
    void drawDebug2D(Matrix4& proj) const;

    //3D関連の描画
    void draw3D(
        const Renderer& renderer,
        const Matrix4& viewProj
    ) const;

private:
    DebugManager(const DebugManager&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;

    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::unique_ptr<DrawString> mStringDrawer;
};
