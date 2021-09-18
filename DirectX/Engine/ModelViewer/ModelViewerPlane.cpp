#include "ModelViewerPlane.h"
#include "../DebugManager/Inspector/ImGuiWrapper.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../GameObject/GameObject.h"
#include "../../Mesh/MeshManager.h"
#include "../../Transform/Transform3D.h"

ModelViewerPlane::ModelViewerPlane()
    : mPlane(nullptr)
    , mIsDraw(true)
{
}

ModelViewerPlane::~ModelViewerPlane() = default;

void ModelViewerPlane::initialize(
    MeshManager& meshManager,
    IModelViewerCallback* callback
) {
    callback->callbackModelChange([&](const GameObject& newModel) { onChangeModel(newModel); });

    mPlane = std::make_shared<GameObject>();
    //床メッシュ作成
    auto mc = Component::addComponent<MeshComponent>(*mPlane, "MeshComponent");
    mc->createMesh("Plane_ascii.fbx", "Assets\\Model\\");
    auto mr = mc->getComponent<MeshRenderer>();
    //床を管理者に追加
    meshManager.add(mr, true);
}

void ModelViewerPlane::update() {
    mPlane->update();
    mPlane->lateUpdate();
}

void ModelViewerPlane::drawGUI() {
    ImGui::Text("Plane");

    if (ImGui::Checkbox("IsDraw", &mIsDraw)) {
        mPlane->setActive(mIsDraw);
    }

    auto& t = mPlane->transform();
    auto posY = t.getPosition().y;
    if (ImGuiWrapper::dragFloat("PositionY", posY, 0.01f)) {
        t.setPosition(Vector3::up * posY);
    }
}

void ModelViewerPlane::onChangeModel(const GameObject& newModel) {
    //床の位置となるメッシュのY軸最低値を求める
    auto mesh = newModel.componentManager().getComponent<MeshComponent>()->getMesh();
    auto meshCount = mesh->getMeshCount();
    float minY = FLT_MAX;
    for (unsigned i = 0; i < meshCount; ++i) {
        const auto& vertices = mesh->getMeshVerticesPosition(i);
        for (const auto& v : vertices) {
            if (v.y < minY) {
                minY = v.y;
            }
        }
    }

    //位置を決定
    mPlane->transform().setPosition(Vector3::up * minY);
}
