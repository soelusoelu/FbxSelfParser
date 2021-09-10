#include "ModelViewerColliderManager.h"
#include "ColliderOptionGUI.h"
#include "../Camera/SimpleCamera.h"
#include "../DebugManager/DebugUtility/LineRenderer/LineInstancingDrawer.h"
#include "../DebugManager/Inspector/ImGuiWrapper.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Collider/ColliderDrawer.h"
#include "../../Component/Engine/Collider/OBBCollider.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../Component/Engine/Mesh/SkinMeshComponent.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"

ModelViewerColliderManager::ModelViewerColliderManager()
    : mOptionGui(std::make_unique<ColliderOptionGUI>(mObbColliders))
    , mMesh(nullptr)
    , mSkinMesh(nullptr)
    , mSelectedObbNo(INVALID_NO)
    , mIsDrawBone(true)
{
    mOptionGui->onOkButton([&] { mSelectedObbNo = -1; });
}

ModelViewerColliderManager::~ModelViewerColliderManager() = default;

void ModelViewerColliderManager::initialize(IModelViewerCallback* callback) {
    callback->callbackModeChange([&](ModelViewerMode mode) { onModeChange(mode); });
    callback->callbackModelChange([&](GameObject& newModel) { onChangeModel(newModel); });
}

void ModelViewerColliderManager::update(LineInstancingDrawer& line, const SimpleCamera& camera) {
    drawBone(line);
    selectObb(camera);

    if (Input::keyboard().getKeyDown(KeyCode::P) && mSelectedObbNo != INVALID_NO) {
        mOptionGui->open(mObbColliders[mSelectedObbNo]);
    }

    const auto size = mObbColliders.size();
    for (unsigned i = 0; i < size; ++i) {
        Vector3 color = ColorPalette::lightGreen;
        if (i == mSelectedObbNo) {
            color = ColorPalette::red;
        }

        ColliderDrawer::drawOBB(line, mObbColliders[i]->getOBB(), color);
    }
}

void ModelViewerColliderManager::drawGUI() {
    ImGui::Checkbox("IsDrawBone", &mIsDrawBone);
    if (mSelectedObbNo != INVALID_NO) {
        mObbColliders[mSelectedObbNo]->drawInspector();
    }

    mOptionGui->drawGui();
}

void ModelViewerColliderManager::drawBone(LineInstancingDrawer& line) const {
    if (!isAnimation()) {
        return;
    }
    if (!mIsDrawBone) {
        return;
    }

    const auto animation = mMesh->getAnimation();
    const auto boneCount = animation->getBoneCount();

    const auto& curBones = mSkinMesh->getBoneCurrentFrameMatrix();
    for (unsigned i = 0; i < boneCount; ++i) {
        const auto& bone = animation->getBone(i);
        const auto parent = bone.parent;
        //親がいないなら次へ
        if (!parent) {
            continue;
        }

        //自身のボーンと親のボーンを線で繋ぐ
        line.add(
            Vector3::transform(bone.initMat.getTranslation(), curBones[bone.number]),
            Vector3::transform(parent->initMat.getTranslation(), curBones[parent->number]),
            ColorPalette::yellow
        );
    }
}

void ModelViewerColliderManager::createObbCollider() {
    mObbColliders = mMesh->getComponents<OBBCollider>();

    //元があるなら終了
    if (mObbColliders.size() > 0) {
        return;
    }
    //アニメーションしないなら終了
    if (!isAnimation()) {
        return;
    }

    const auto animation = mMesh->getAnimation();
    const auto boneCount = animation->getBoneCount();
    for (unsigned i = 0; i < boneCount; ++i) {
        //ボーンの親がいないなら次へ
        if (!animation->getBone(i).parent) {
            continue;
        }

        auto obb = mMesh->addComponent<OBBCollider>("OBBCollider");
        obb->setBone(i);
        mObbColliders.emplace_back(obb);
    }
}

void ModelViewerColliderManager::selectObb(const SimpleCamera& camera) {
    const auto& mouse = Input::mouse();
    const auto& mousePos = mouse.getMousePosition();
    if (mousePos.x > Window::width()) {
        return;
    }
    if (mousePos.y > Window::height()) {
        return;
    }

    const auto ray = camera.screenToRay(mousePos);
    const auto size = mObbColliders.size();
    for (unsigned i = 0; i < size; ++i) {
        if (Intersect::intersectRayOBB(ray, mObbColliders[i]->getOBB())) {
            if (mouse.getMouseButtonDown(MouseCode::LeftButton)) {
                mSelectedObbNo = i;
                return;
            }
        }
    }
}

bool ModelViewerColliderManager::isAnimation() const {
    return (mSkinMesh) ? true : false;
}

void ModelViewerColliderManager::onChangeModel(const GameObject& newModel) {
    const auto& cm = newModel.componentManager();
    mMesh = cm.getComponent<MeshComponent>();
    mSkinMesh = cm.getComponent<SkinMeshComponent>();

    if (mObbColliders.size() > 0) {
        mObbColliders.clear();
    }
}

void ModelViewerColliderManager::onModeChange(ModelViewerMode mode) { 
    FillMode fillMode = FillMode::SOLID;

    if (mode == ModelViewerMode::COLLIDER_OPERATE) {
        if (isAnimation()) {
            //強制的にTポーズに変更する
            mSkinMesh->tPose();
        }

        //最初はボーンを表示する
        mIsDrawBone = true;

        //OBB作成
        createObbCollider();

        //ワイヤーフレーム表示に変更する
        fillMode = FillMode::WIREFRAME;
    }

    //描画方法を変更する
    mMesh->getComponent<MeshRenderer>()->setFillMode(fillMode);
}
