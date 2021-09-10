#include "AABBAnimationCollider.h"
#include "../Mesh/AnimationCPU.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Device/Physics.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Transform/Transform3D.h"
#include <cassert>

AABBAnimationCollider::AABBAnimationCollider()
    : Collider()
    , mMesh(nullptr)
    , mAnimationCPU(nullptr)
    , mIsRenderCollision(true)
{
}

AABBAnimationCollider::~AABBAnimationCollider() = default;

ColliderType AABBAnimationCollider::getType() const {
    return ColliderType::AABB;
}

void AABBAnimationCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    mAnimationCPU = getComponent<AnimationCPU>();

    //メッシュの数分拡張する
    unsigned meshCount = mMesh->getMesh()->getMeshCount();
    mAABBs.resize(meshCount);
    //すべてのメッシュからAABBを作成する
    for (unsigned i = 0; i < meshCount; ++i) {
        createAABB(i);
    }

    //最新のAABBの点を計算する
    updatePoints();

    //if (mPhysics) {
    //    mPhysics->add(shared_from_this());
    //}
}

void AABBAnimationCollider::lateUpdate() {
    Collider::lateUpdate();

    //AABBを更新する
    computeAABB();

    //AABBの点を更新する
    updatePoints();

    //当たり判定を可視化する
    renderCollision();
}

void AABBAnimationCollider::finalize() {
    Collider::finalize();

    //if (mPhysics) {
    //    mPhysics->remove(shared_from_this());
    //}
}

void AABBAnimationCollider::onEnable(bool value) {
    Collider::onEnable(value);

    setRenderCollision(value);
}

void AABBAnimationCollider::drawInspector() {
    Collider::drawInspector();

    ImGui::Checkbox("IsRenderCollision", &mIsRenderCollision);
}

void AABBAnimationCollider::concatenate(unsigned a, unsigned b) {
    auto size = mAABBs.size();
    assert(a < size);
    assert(b < size);

    mAABBs[a].concatenateTargets.emplace_back(b);
    mAABBs[a].isActive = true;
    mAABBs[b].isActive = false;
}

const AABBs& AABBAnimationCollider::getAABBs() const {
    return mAABBs;
}

const AABB& AABBAnimationCollider::getAABB(unsigned index) const {
    assert(index < mAABBs.size());
    return mAABBs[index].aabb;
}

void AABBAnimationCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void AABBAnimationCollider::computeAABB() {
    //すべてのメッシュからAABBを作成する
    for (size_t i = 0; i < mAABBs.size(); ++i) {
        const auto& target = mAABBs[i];
        if (!target.isActive) {
            continue;
        }
        //スキニングの結果からAABBを作成する
        createAABB(i);

        const auto& targets = target.concatenateTargets;
        for (const auto& t : targets) {
            //AABBを拡張する
            updateAABB(i, t);
        }

        //transformに適応する
        adaptAABBToTransform(i);
    }
}

void AABBAnimationCollider::updateAABB(unsigned target, unsigned index) {
    mAABBs[target].aabb.updateMinMax(createdAABB(index));
}

void AABBAnimationCollider::createAABB(unsigned index) {
    mAABBs[index].aabb = createdAABB(index);
}

void AABBAnimationCollider::adaptAABBToTransform(unsigned index) {
    const auto& t = transform();
    const auto& pos = t.getPosition();
    const auto& scale = t.getScale();
    //AABBをtransformの値から更新する
    auto& target = mAABBs[index].aabb;
    target.min *= scale;
    target.max *= scale;
    target.rotate(t.getRotation());
    target.min += pos;
    target.max += pos;
}

AABB AABBAnimationCollider::createdAABB(unsigned index) const {
    return AABBCreater::create(mAnimationCPU->getCurrentMotionVertexPositions(index));
}

void AABBAnimationCollider::updatePoints() {
    for (auto&& target : mAABBs) {
        if (!target.isActive) {
            continue;
        }

        const auto& aabb = target.aabb;
        const auto& min = aabb.min;
        const auto& max = aabb.max;

        auto& points = target.points;
        points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT] = min;
        points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT] = Vector3(max.x, min.y, min.z);
        points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT] = Vector3(min.x, min.y, max.z);
        points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT] = Vector3(max.x, min.y, max.z);
        points[BoxConstantGroup::BOX_FORE_TOP_LEFT] = Vector3(min.x, max.y, min.z);
        points[BoxConstantGroup::BOX_FORE_TOP_RIGHT] = Vector3(max.x, max.y, min.z);
        points[BoxConstantGroup::BOX_BACK_TOP_LEFT] = Vector3(min.x, max.y, max.z);
        points[BoxConstantGroup::BOX_BACK_TOP_RIGHT] = max;
    }
}

void AABBAnimationCollider::renderCollision() {
#ifdef _DEBUG
    if (!mIsRenderCollision) {
        return;
    }
    if (!mEnable) {
        return;
    }

    //デバッグ時のみ当たり判定を表示
    for (const auto& target : mAABBs) {
        if (!target.isActive) {
            continue;
        }

        const auto& points = target.points;
        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT], points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT], points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT], points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], ColorPalette::lightGreen);

        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_TOP_LEFT], points[BoxConstantGroup::BOX_FORE_TOP_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_TOP_LEFT], points[BoxConstantGroup::BOX_BACK_TOP_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_TOP_LEFT], points[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_TOP_RIGHT], points[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);

        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT], points[BoxConstantGroup::BOX_FORE_TOP_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT], points[BoxConstantGroup::BOX_FORE_TOP_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], points[BoxConstantGroup::BOX_BACK_TOP_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], points[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);
    }
#endif // _DEBUG
}
