#include "SceneManager.h"
#include "Texture/MeshRenderOnTextureManager.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../Engine/EngineFunctionManager.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"

SceneManager::SceneManager()
    : FileOperator("SceneManager")
    , mRenderer(std::make_unique<Renderer>())
    , mEngineManager(std::make_unique<EngineFunctionManager>())
    , mMeshManager(std::make_unique<MeshManager>())
    , mSpriteManager(std::make_unique<SpriteManager>())
    , mMeshRenderOnTextureManager(std::make_unique<MeshRenderOnTextureManager>())
    , mTextDrawer(std::make_unique<DrawString>())
{
}

SceneManager::~SceneManager() = default;

void SceneManager::initialize() {
    mRenderer->initialize();
    mEngineManager->initialize();
    mMeshManager->initialize();
    mTextDrawer->initialize();
}

void SceneManager::update() {
    //アップデート前処理
    mEngineManager->preUpdateProcess();
    //デバッグ
    mEngineManager->update();

    //スプライトはいつでも更新する
    mSpriteManager->update();
    mMeshRenderOnTextureManager->update();
}

void SceneManager::draw() const {
    //メッシュ描画準備
    mRenderer->renderMesh();
    mEngineManager->draw3D(*mRenderer);

    //スプライト描画準備
    auto proj2D = Matrix4::identity;
    mRenderer->renderSprite();
    mRenderer->renderSprite2D(proj2D);

    //レンダリング領域をデバッグに変更
    mRenderer->renderToDebug(proj2D);
    mSpriteManager->draw(proj2D);
    mEngineManager->drawDebug2D(proj2D);
}

void SceneManager::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mEngineManager->writeAndRead(inObj, alloc, mode);
    mMeshManager->saveAndLoad(inObj, alloc, mode);
    mTextDrawer->writeAndRead(inObj, alloc, mode);
}
