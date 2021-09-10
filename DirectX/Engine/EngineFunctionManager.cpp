#include "EngineFunctionManager.h"
#include "AssetsRenderer/AssetsRenderTextureManager.h"
#include "DebugManager/DebugManager.h"
#include "ModelViewer/ModelViewer.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Device/Renderer.h"

EngineFunctionManager::EngineFunctionManager()
    : FileOperator("EngineFunctionManager")
    , mDebugManager(std::make_unique<DebugManager>())
    , mAssetsRenderTextureManager(std::make_unique<AssetsRenderTextureManager>())
    , mModelViewer(std::make_unique<ModelViewer>())
{
}

EngineFunctionManager::~EngineFunctionManager() = default;

DebugManager& EngineFunctionManager::debug() const {
    return *mDebugManager;
}

AssetsRenderTextureManager& EngineFunctionManager::getAssetsRenderTextureManager() const {
    return *mAssetsRenderTextureManager;
}

void EngineFunctionManager::initialize() {
    mDebugManager->initialize();
    mAssetsRenderTextureManager->initialize();
    mModelViewer->initialize(
        mAssetsRenderTextureManager.get(),
        mAssetsRenderTextureManager->getCallbackSelectAssetsTexture(),
        this
    );
}

void EngineFunctionManager::preUpdateProcess() {
    mDebugManager->preUpdateProcess();
}

void EngineFunctionManager::update() {
    mAssetsRenderTextureManager->update();
    mModelViewer->update();
}

void EngineFunctionManager::draw2D(const Renderer& renderer, Matrix4& proj) const {
    mDebugManager->draw2D(renderer, proj);
}

void EngineFunctionManager::drawDebug2D(Matrix4& proj) const {
    mAssetsRenderTextureManager->drawTextures(proj);
    mDebugManager->drawDebug2D(proj);
}

void EngineFunctionManager::draw3D(const Renderer& renderer) const {
    mAssetsRenderTextureManager->drawMeshes();
    mModelViewer->draw(renderer);
}

void EngineFunctionManager::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mDebugManager->writeAndRead(inObj, alloc, mode);
    mAssetsRenderTextureManager->saveAndLoad(inObj, alloc, mode);
    mModelViewer->saveAndLoad(inObj, alloc, mode);
}
