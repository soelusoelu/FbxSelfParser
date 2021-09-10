#include "OBBAnimationCollider.h"
#include "../Mesh/AnimationCPU.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Transform/Transform3D.h"
#include <cassert>

OBBAnimationCollider::OBBAnimationCollider()
    : Collider()
    , mMesh(nullptr)
    , mAnimationCPU(nullptr)
    , mIsRenderCollision(true)
{
}

OBBAnimationCollider::~OBBAnimationCollider() = default;

ColliderType OBBAnimationCollider::getType() const {
    return ColliderType::OBB;
}

void OBBAnimationCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    mAnimationCPU = getComponent<AnimationCPU>();

    //メッシュの数分拡張する
    unsigned meshCount = mMesh->getMesh()->getMeshCount();
    mOBBs.resize(meshCount);
    //すべてのメッシュからOBBを作成する
    for (unsigned i = 0; i < meshCount; ++i) {
        createOBB(i);
    }

    if (mPhysics) {
        //mPhysics->add(shared_from_this());
    }
}

void OBBAnimationCollider::lateUpdate() {
    Collider::lateUpdate();

    //OBBを更新する
    computeOBB();

    //当たり判定を可視化する
    if (mIsRenderCollision) {
        renderCollision();
    }
}

void OBBAnimationCollider::finalize() {
    Collider::finalize();

    if (mPhysics) {
        //mPhysics->remove(shared_from_this());
    }
}

void OBBAnimationCollider::onEnable(bool value) {
    Collider::onEnable(value);

    setRenderCollision(value);
}

void OBBAnimationCollider::drawInspector() {
    Collider::drawInspector();

    ImGui::Checkbox("IsRenderCollision", &mIsRenderCollision);
}

void OBBAnimationCollider::concatenate(unsigned a, unsigned b) {
    auto size = mOBBs.size();
    assert(a < size);
    assert(b < size);

    auto& obbA = mOBBs[a];
    obbA.concatenateTargets.emplace_back(b);
    obbA.isActive = true;
    mOBBs[b].isActive = false;
}

const OBB& OBBAnimationCollider::getOBB(unsigned index) const {
    assert(index < mOBBs.size());
    return mOBBs[index].obb;
}

void OBBAnimationCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void OBBAnimationCollider::computeOBB() {
    //すべてのメッシュからOBBを作成する
    auto size = mOBBs.size();
    for (size_t i = 0; i < size; ++i) {
        const auto& target = mOBBs[i];
        if (!target.isActive) {
            continue;
        }
        //createOBB(i);
        updateOBB(i, i);

        //const auto& targets = target.concatenateTargets;
        //for (const auto& t : targets) {
        //    updateOBB(i, t);
        //}
    }
}

void OBBAnimationCollider::updateOBB(unsigned target, unsigned index) {
    //スキニング結果から更新する
    const auto& positions = mAnimationCPU->getCurrentMotionVertexPositions(index);
    mOBBs[target].obb = OBBCreater::create(positions);
}

void OBBAnimationCollider::createOBB(unsigned meshIndex) {
    const auto& positions = mMesh->getMesh()->getMeshVerticesPosition(meshIndex);
    mOBBs[meshIndex].obb = OBBCreater::create(positions);
}

void OBBAnimationCollider::renderCollision() {
#ifdef _DEBUG
    //デバッグ時のみ当たり判定を表示
    for (const auto& target : mOBBs) {
        if (!target.isActive) {
            continue;
        }

        const auto& obb = target.obb;
        const auto& center = obb.center;
        const auto& rot = obb.rotation;
        const auto& extents = obb.extents;

        Vector3 ftr = extents;
        ftr = Vector3::transform(ftr, rot) + center;
        Vector3 fbr = Vector3(extents.x, -extents.y, extents.z);
        fbr = Vector3::transform(fbr, rot) + center;
        Vector3 ftl = Vector3(-extents.x, extents.y, extents.z);
        ftl = Vector3::transform(ftl, rot) + center;
        Vector3 fbl = Vector3(-extents.x, -extents.y, extents.z);
        fbl = Vector3::transform(fbl, rot) + center;

        Vector3 btr = Vector3(extents.x, extents.y, -extents.z);
        btr = Vector3::transform(btr, rot) + center;
        Vector3 bbr = Vector3(extents.x, -extents.y, -extents.z);
        bbr = Vector3::transform(bbr, rot) + center;
        Vector3 btl = Vector3(-extents.x, extents.y, -extents.z);
        btl = Vector3::transform(btl, rot) + center;
        Vector3 bbl = -extents;
        bbl = Vector3::transform(bbl, rot) + center;

        Debug::renderLine(ftr, fbr, ColorPalette::lightGreen);
        Debug::renderLine(ftr, ftl, ColorPalette::lightGreen);
        Debug::renderLine(ftl, fbl, ColorPalette::lightGreen);
        Debug::renderLine(fbl, fbr, ColorPalette::lightGreen);

        Debug::renderLine(btr, bbr, ColorPalette::lightGreen);
        Debug::renderLine(btr, btl, ColorPalette::lightGreen);
        Debug::renderLine(btl, bbl, ColorPalette::lightGreen);
        Debug::renderLine(bbl, bbr, ColorPalette::lightGreen);

        Debug::renderLine(ftr, btr, ColorPalette::lightGreen);
        Debug::renderLine(ftl, btl, ColorPalette::lightGreen);
        Debug::renderLine(fbr, bbr, ColorPalette::lightGreen);
        Debug::renderLine(fbl, bbl, ColorPalette::lightGreen);

        //Debug::renderLine(center, center + Vector3::right, ColorPalette::red);
        //Debug::renderLine(center, center + Vector3::up, ColorPalette::blue);
        //Debug::renderLine(center, center + Vector3::back, ColorPalette::green);
    }
#endif // _DEBUG
}
