#include "ModelViewCamera.h"
#include "../Camera/EngineCamera.h"
#include "../Camera/SimpleCamera.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"

ModelViewCamera::ModelViewCamera()
    : mCamera(std::make_unique<EngineCamera>())
    , mModelCenterView()
{
}

ModelViewCamera::~ModelViewCamera() = default;

void ModelViewCamera::initialize(IModelViewerCallback* callback) {
    callback->callbackModelChange([&](GameObject& newModel) { onChangeModel(newModel); });

    //射影行列を求め、カメラに設定する
    mCamera->getCamera().setProjection(Matrix4::createPerspectiveFOV(
        Window::width(),
        Window::height(),
        FOV,
        NEAR_CLIP,
        FAR_CLIP
    ));
}

void ModelViewCamera::update() {
    mCamera->update();

    if (Input::keyboard().getKeyDown(KeyCode::F)) {
        setModelCenterPosition();
    }
}

void ModelViewCamera::setModelCenterPosition() {
    mCamera->getCamera().setView(mModelCenterView);
}

const SimpleCamera& ModelViewCamera::getCamera() const {
    return mCamera->getCamera();
}

void ModelViewCamera::onChangeModel(const GameObject& newModel) {
    //カメラ状態初期化
    mCamera->initialize();

    auto& camera = mCamera->getCamera();

    //モデルを包む球を求める
    auto mesh = newModel.componentManager().getComponent<MeshComponent>()->getMesh();
    Sphere sphere;
    SphereHelper::create(sphere, *mesh);

    //モデル全体を映せるカメラ位置を設定する
    camera.setPosition(CameraHelper::getCameraPositionTakingSphereInCamera(
        sphere,
        Window::width(),
        Window::height(),
        FOV,
        Vector3::forward
    ));
    //注視点をモデルの中心に設定する
    camera.lookAt(sphere.center);

    //モデル全体を映すビュー行列を保存する
    mModelCenterView = camera.getView();
}
