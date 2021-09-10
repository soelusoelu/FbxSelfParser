#include "Collider.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Imgui/imgui.h"
#include <algorithm>

Collider::Collider()
    : Component()
    , mIsAutoUpdate(true)
    , mEnable(false)
{
}

Collider::~Collider() = default;

void Collider::start() {
    mEnable = true;
}

void Collider::lateUpdate() {
    //衝突し続けている/衝突しなくなった コライダーに通知を送る
    notifyCollisionStay();
    notifyCollisionExit();

    //衝突したコライダーを移す
    mPreviousCollider.resize(mCurrentCollider.size());
    std::copy(mCurrentCollider.begin(), mCurrentCollider.end(), mPreviousCollider.begin());
    mCurrentCollider.clear();
}

void Collider::finalize() {
    mPreviousCollider.clear();
    mCurrentCollider.clear();
}

void Collider::drawInspector() {
    ImGui::Checkbox("IsAutoUpdate", &mIsAutoUpdate);
    ImGui::Checkbox("Enable", &mEnable);
}

void Collider::onEnable(bool value) {
    (value) ? enabled() : disabled();
}

void Collider::enabled() {
    mEnable = true;
}

void Collider::disabled() {
    mEnable = false;
}

bool Collider::getEnable() const {
    return mEnable;
}

void Collider::automation() {
    if (!mIsAutoUpdate) {
        mIsAutoUpdate = true;
    }
}

void Collider::addHitCollider(const CollPtr& hit) {
    mCurrentCollider.emplace_back(hit);

    //追加されたヒットコライダーが新しいものなら
    if (isCollisionEnter(hit)) {
        gameObject().componentManager().onCollisionEnter(*hit);
    }
}

void Collider::setPhysics(Physics* physics) {
    mPhysics = physics;
}

void Collider::notifyCollisionStay() const {
    for (const auto& curColl : mCurrentCollider) {
        //1フレームの間に状態が変わってるかもしれないからチェック
        if (!curColl->gameObject().getActive()) {
            continue;
        }
        if (isCollisionStay(curColl)) {
            gameObject().componentManager().onCollisionStay(*curColl);
        }
    }
}

void Collider::notifyCollisionExit() const {
    for (const auto& preColl : mPreviousCollider) {
        //1フレームの間に状態が変わってるかもしれないからチェック
        if (!preColl->gameObject().getActive()) {
            continue;
        }
        if (isCollisionExit(preColl)) {
            gameObject().componentManager().onCollisionExit(*preColl);
        }
    }
}

bool Collider::isCollisionEnter(const CollPtr& hit) const {
    auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), hit);
    return (itr == mPreviousCollider.end());
}

bool Collider::isCollisionStay(const CollPtr& hit) const {
    auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), hit);
    return (itr != mPreviousCollider.end());
}

bool Collider::isCollisionExit(const CollPtr& hit) const {
    auto itr = std::find(mCurrentCollider.begin(), mCurrentCollider.end(), hit);
    return (itr == mCurrentCollider.end());
}
