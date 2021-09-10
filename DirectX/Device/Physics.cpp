#include "Physics.h"
#include "../Collision/Collision.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Collider/OBBCollider.h"
#include <algorithm>

Physics::Physics() {
    Collider::setPhysics(this);
}

Physics::~Physics() {
    Collider::setPhysics(nullptr);
}

void Physics::add(const ColliderPtr& collider) {
    mColliders.emplace_back(collider);
}

void Physics::remove(const ColliderPtr& collider) {
    auto itr = std::find(mColliders.begin(), mColliders.end(), collider);
    if (itr != mColliders.end()) {
        std::iter_swap(itr, mColliders.end() - 1);
        mColliders.pop_back();
    }
}

void Physics::clear() {
    mColliders.clear();
}

void Physics::sweepAndPrune() {
    if (mColliders.empty()) {
        return;
    }

    auto size = mColliders.size();
    for (size_t i = 0; i < size; ++i) {
        const auto& aColl = mColliders[i];
        if (!enabled(*aColl)) {
            continue;
        }

        const auto aObb = static_cast<const OBBCollider*>(&*aColl);
        const auto& a = aObb->getOBB();

        for (size_t j = i + 1; j < size; ++j) {
            const auto& bColl = mColliders[j];
            if (!enabled(*bColl)) {
                continue;
            }
            //同一ゲームオブジェクトのコライダーなら次へ
            if (&aColl->gameObject() == &bColl->gameObject()) {
                break;
            }

            const auto bObb = static_cast<const OBBCollider*>(&*bColl);
            const auto& b = bObb->getOBB();

            //衝突判定
            if (Intersect::intersectOBB(a, b)) {
                aColl->addHitCollider(bColl);
                bColl->addHitCollider(aColl);
            }
        }
    }
}

bool Physics::enabled(const Collider& collider) {
    if (!collider.gameObject().getActive()) {
        return false;
    }
    if (!collider.getEnable()) {
        return false;
    }

    return true;
}
