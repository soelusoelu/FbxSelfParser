#include "AnimationViewer.h"
#include "../../Component/Engine/Mesh/SkinMeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"

AnimationViewer::AnimationViewer()
    : mAnimation(nullptr)
{
}

AnimationViewer::~AnimationViewer() = default;

void AnimationViewer::initialize(IModelViewerCallback* callback) {
    callback->callbackModelChange([&](GameObject& newModel) { onChangeModel(newModel); });
}

void AnimationViewer::update() {
    if (!mAnimation) {
        return;
    }

    int nextMotionNumber = -1;
    if (isChangeMotion(nextMotionNumber)) {
        //モーションを変更する
        mAnimation->changeMotion(nextMotionNumber);
    }

    //Tポーズ
    setTPose();
}

void AnimationViewer::setTPose() {
    if (Input::keyboard().getKeyDown(KeyCode::Alpha0)) {
        mAnimation->tPose();
    }
}

bool AnimationViewer::isChangeMotion(int& nextMotionNumber) const {
    int keyNumber = Input::keyboard().getNumber();
    //数字キーが押されてなければ終了
    if (keyNumber == Keyboard::INVALID_NUMBER) {
        return false;
    }
    //数字キーが0なら終了(Tポーズ用)
    if (keyNumber == 0) {
        return false;
    }

    int motionCount = mAnimation->getMotionCount();
    int nextNum = keyNumber - 1;
    //数字がモーションの数以上なら終了
    if (nextNum >= motionCount) {
        return false;
    }
    //今のモーションと一緒なら終了
    if (nextNum == mAnimation->getCurrentMotionNumber()) {
        return false;
    }

    nextMotionNumber = nextNum;
    return true;
}

void AnimationViewer::onChangeModel(const GameObject& newModel) {
    mAnimation = newModel.componentManager().getComponent<SkinMeshComponent>();
    if (mAnimation) {
        //初期姿勢はTポーズ
        mAnimation->tPose();
    }
}
