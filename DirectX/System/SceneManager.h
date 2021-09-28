#pragma once

#include "../Device/FileOperator.h"
#include <memory>

class Renderer;
class EngineFunctionManager;
class MeshManager;
class SpriteManager;
class MeshRenderOnTextureManager;
class DrawString;

class SceneManager
    : public FileOperator
{
public:
    SceneManager();
    ~SceneManager();

    virtual void childSaveAndLoad(JsonObject& inObj, FileMode mode) override;

    void initialize();
    void update();
    void draw() const;

private:
    std::unique_ptr<Renderer> mRenderer;
    std::unique_ptr<EngineFunctionManager> mEngineManager;
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<SpriteManager> mSpriteManager;
    std::unique_ptr<MeshRenderOnTextureManager> mMeshRenderOnTextureManager;
    std::unique_ptr<DrawString> mTextDrawer;
};
