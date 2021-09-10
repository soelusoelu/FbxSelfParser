#include "ModelViewer.h"
#include "AnimationViewer.h"
#include "ModelViewCamera.h"
#include "ModelViewerColliderManager.h"
#include "ModelViewerLight.h"
#include "ModelViewerPlane.h"
#include "SaveModelViewerCollider.h"
#include "../AssetsRenderer/AssetsRenderTexture.h"
#include "../Camera/SimpleCamera.h"
#include "../DebugManager/DebugManager.h"
#include "../DebugManager/DebugUtility/LineRenderer/LineInstancingDrawer.h"
#include "../DebugManager/Inspector/ImGuiWrapper.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../Device/Renderer.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Mesh/MeshManager.h"
#include "../../System/Window.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/FileUtil.h"

ModelViewer::ModelViewer()
    : mAssetsTextureGetter(nullptr)
    , mMeshManager(std::make_unique<MeshManager>())
    , mLineRenderer(std::make_unique<LineInstancingDrawer>())
    , mPlane(std::make_unique<ModelViewerPlane>())
    , mModelViewCamera(std::make_unique<ModelViewCamera>())
    , mLight(std::make_unique<ModelViewerLight>())
    , mAnimationViewer(std::make_unique<AnimationViewer>())
    , mColliderManager(std::make_unique<ModelViewerColliderManager>())
    , mMode(ModelViewerMode::MODEL_VIEW)
    , mIsUpdate(true)
{
}

ModelViewer::~ModelViewer() = default;

void ModelViewer::callbackModeChange(const std::function<void(ModelViewerMode)>& f) {
    mCallbackModeChange += f;
}

void ModelViewer::callbackModelChange(const std::function<void(GameObject&)>& f) {
    mCallbackModelChange += f;
}

void ModelViewer::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::LOAD) {
        mMeshManager->saveAndLoad(inObj, alloc, mode);
    }

    mLight->writeAndRead(inObj, alloc, mode);
}

void ModelViewer::initialize(
    const ICurrentSelectTextureGetter* assetsTextureGetter,
    ICallbackSelectAssetsTexture* callbackSelectAssetsTexture,
    IEngineManagingClassGetter* engineManagingClassGetter
) {
    mAssetsTextureGetter = assetsTextureGetter;

    mMeshManager->initialize();
    mLineRenderer->initialize();
    mModelViewCamera->initialize(this);
    mAnimationViewer->initialize(this);
    mPlane->initialize(*mMeshManager, this);
    mColliderManager->initialize(this);

    callbackSelectAssetsTexture->callbackSelectTexture([&] { onSelectAssetsTexture(); });
    mMeshManager->registerThisToMeshRenderer();
}

void ModelViewer::update() {
    //バッファ削除
    mLineRenderer->clear();

    if (!mTarget.first) {
        return;
    }

    //タブキーが押されたらモードを切り替える
    if (Input::keyboard().getKeyDown(KeyCode::Tab)) {
        changeMode(
            (mMode == ModelViewerMode::MODEL_VIEW)
            ? ModelViewerMode::COLLIDER_OPERATE
            : ModelViewerMode::MODEL_VIEW
        );
    }

    //選択されているモードに応じて処理を分ける
    if (mMode == ModelViewerMode::COLLIDER_OPERATE) {
        mColliderManager->update(*mLineRenderer, mModelViewCamera->getCamera());
    }

    //全モード共通処理
    if (Input::keyboard().getKeyDown(KeyCode::Space)) {
        mIsUpdate = !mIsUpdate;
    }
    if (mIsUpdate) {
        if (mTarget.first) {
            mTarget.first->update();
            mTarget.first->lateUpdate();
        }
    }
    mPlane->update();
    mAnimationViewer->update();
    mModelViewCamera->update();

    saveModel();
}

void ModelViewer::setTarget(
    const GameObjectPtr& targetObj,
    const MeshRendererPtr& targetMesh
) {
    if (mTarget.second) {
        mMeshManager->erase(mTarget.second);
    }

    mTarget = std::make_pair(targetObj, targetMesh);
    mMeshManager->add(targetMesh, true);

    mCallbackModelChange(*targetObj);

    //強制的にビューモードにする
    changeMode(ModelViewerMode::MODEL_VIEW);
    mIsUpdate = true;
}

void ModelViewer::draw(const Renderer& renderer) const {
    //パラメータ調整用GUI描画
    drawGUI();

    //モデル描画
    const auto& camera = mModelViewCamera->getCamera();
    mMeshManager->draw(
        camera.getView(),
        camera.getProjection(),
        camera.getPosition(),
        mLight->getDirection(),
        mLight->getColor()
    );

    //ライン描画
    renderer.renderPointLine3D();
    renderer.renderLine3D();
    mLineRenderer->draw(camera.getViewProjection());
}

void ModelViewer::drawGUI() const {
    //マウス位置を補正する
    const auto& mousePos = Input::mouse().getMousePosition();
    ImGui::GetIO().MousePos = ImVec2(mousePos.x, mousePos.y);
    //ウィンドウ位置を固定
    ImGui::SetNextWindowPos(ImVec2(Window::width(), 0.f), ImGuiCond_Always);
    //ウィンドウサイズを固定
    ImGui::SetNextWindowSize(ImVec2(Window::standardWidth() - Window::width(), Window::standardHeight()), ImGuiCond_Always);

    ImGui::Begin("ModelViewer");

    mLight->drawGUI();
    ImGui::Separator(); //区切り線
    mPlane->drawGUI();
    if (mMode == ModelViewerMode::COLLIDER_OPERATE) {
        ImGui::Separator(); //区切り線
        mColliderManager->drawGUI();
    }

    ImGui::End();
}

void ModelViewer::changeMode(ModelViewerMode mode) {
    mMode = mode;
    mCallbackModeChange(mode);
}

void ModelViewer::saveModel() {
    const auto& keyboard = Input::keyboard();
    bool pressedS = keyboard.getKeyDown(KeyCode::S);
    bool pressedLeftControl = keyboard.getKey(KeyCode::LeftControl);
    if (pressedLeftControl && pressedS) {
        if (!mTarget.first) {
            return;
        }

        SaveModelViewerCollider::save(*mTarget.first);
    }
}

void ModelViewer::onSelectAssetsTexture() {
    if (!mAssetsTextureGetter->selectedTexture()) {
        return;
    }

    GameObjectPtr newTarget = nullptr;

    const auto& texFilePath = mAssetsTextureGetter->getCurrentSelectTexture().getTexture().getFilePath();
    const std::string JSON = ".json";
    if (FileUtil::getFileExtension(texFilePath) == JSON) {
        auto filePath = texFilePath.substr(0, texFilePath.length() - JSON.length());
        auto filename = FileUtil::getFileNameFromDirectry(filePath);
        auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);
        newTarget = GameObjectCreater::create(filename, directoryPath);
    } else {
        newTarget = std::make_shared<GameObject>();
        auto newMesh = Component::addComponent<MeshComponent>(*newTarget, "MeshComponent");
        newMesh->createMeshFromFilePath(texFilePath);
    }

    auto meshRenderer = newTarget->componentManager().getComponent<MeshRenderer>();

    setTarget(newTarget, meshRenderer);
}
