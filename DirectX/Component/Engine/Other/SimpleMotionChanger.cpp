#include "SimpleMotionChanger.h"
#include "../Mesh/SkinMeshComponent.h"
#include "../../../Input/Input.h"

SimpleMotionChanger::SimpleMotionChanger()
    : Component()
    , mAnimation(nullptr)
{
}

SimpleMotionChanger::~SimpleMotionChanger() = default;

void SimpleMotionChanger::start() {
    mAnimation = getComponent<SkinMeshComponent>();
}

void SimpleMotionChanger::update() {
    auto keyNumber = Input::keyboard().getNumber();
    //数字キーが押されていないなら終了
    if (keyNumber == Keyboard::INVALID_NUMBER) {
        return;
    }

    //押された数字キーがモーション数を超えていたら終了
    if (keyNumber > mAnimation->getMotionCount()) {
        return;
    }

    //数字キーが0ならTポーズ
    //それ以外は対応するモーションに変更
    if (keyNumber == 0) {
        mAnimation->tPose();
    } else {
        mAnimation->changeMotion(keyNumber - 1);
        mAnimation->setLoop(true);
    }
}
