#include "OBBCollider.h"
#include "ColliderDrawer.h"
#include "../Mesh/MeshComponent.h"
#include "../Mesh/SkinMeshComponent.h"
#include "../../../Device/Physics.h"
#include "../../../Engine/DebugManager/DebugUtility/DebugUtility.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"
#include <cassert>

OBBCollider::OBBCollider()
    : Collider()
    , mOBB()
    , mMesh(nullptr)
    , mAnimation(nullptr)
    , mOffsetCenter(Vector3::zero)
    , mDefaultExtents(Vector3::zero)
    , mBoneNo(-1)
    , mBoneStart(0.f)
    , mBoneEnd(1.f)
{
}

OBBCollider::~OBBCollider() = default;

ColliderType OBBCollider::getType() const {
    return ColliderType::OBB;
}

void OBBCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    mAnimation = getComponent<SkinMeshComponent>();

    mOBB.extents = mDefaultExtents;

    //transform().callbackBeforeComputeWorldMatrix([&] { beforeComputeWorldMatrix(); });

    if (mPhysics) {
        mPhysics->add(shared_from_this());
    }
}

void OBBCollider::lateUpdate() {
    if (!mEnable) {
        return;
    }

    Collider::lateUpdate();

    beforeComputeWorldMatrix();

    //当たり判定表示
    //ColliderDrawer::drawOBB(DebugUtility::instance().lineRenderer3D(), mOBB);
}

void OBBCollider::finalize() {
    Collider::finalize();

    if (mPhysics) {
        mPhysics->remove(shared_from_this());
    }
}

void OBBCollider::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mBoneNo, "boneNo", inObj, alloc, mode);
    JsonHelper::getSet(mBoneStart, "boneStart", inObj, alloc, mode);
    JsonHelper::getSet(mBoneEnd, "boneEnd", inObj, alloc, mode);
    JsonHelper::getSet(mOBB.center, "center", inObj, alloc, mode);
    JsonHelper::getSet(mOBB.rotation, "rotation", inObj, alloc, mode);
    JsonHelper::getSet(mDefaultExtents, "extents", inObj, alloc, mode);
    JsonHelper::getSet(mOffsetCenter, "offsetCenter", inObj, alloc, mode);
}

void OBBCollider::drawInspector() {
    ImGuiWrapper::dragInt("boneNo", mBoneNo);
    ImGuiWrapper::dragFloat("boneStart", mBoneStart, 0.05f, 0.f, mBoneEnd);
    ImGuiWrapper::dragFloat("boneEnd", mBoneEnd, 0.05f, mBoneStart, 1.f);
    ImGuiWrapper::dragVector3("offsetCenter", mOffsetCenter, 0.1f);
    ImGuiWrapper::dragVector3("extents", mDefaultExtents, 0.1f);
}

const OBB& OBBCollider::getOBB() const {
    return mOBB;
}

void OBBCollider::setBone(unsigned boneNo, float start, float end) {
    assert(boneNo < mMesh->getAnimation()->getBoneCount());

    mBoneNo = static_cast<int>(boneNo);
    mBoneStart = start;
    mBoneEnd = end;

    const auto& bone = mMesh->getAnimation()->getBone(mBoneNo);
    const auto parent = bone.parent;
    auto bonePos = bone.initMat.getTranslation();
    auto parentPos = parent->initMat.getTranslation();

    const auto mesh = mMesh->getMesh();
    auto meshCount = mesh->getMeshCount();
    //ボックスを構成する頂点番号群
    std::vector<std::vector<unsigned>> affectedVertices(meshCount);
    std::vector<std::vector<Vector3>> affectedVertices2(meshCount);

    for (unsigned i = 0; i < meshCount; ++i) {
        const auto& vertices = mesh->getMeshVertices(i);
        auto& affected = affectedVertices[i];
        auto& affected2 = affectedVertices2[i];

        //ボーンが影響する頂点を取得する
        const auto size = vertices.size();
        for (unsigned j = 0; j < size; ++j) {
            const auto& v = vertices[j];

            for (int idx = 0; idx < 4; ++idx) {
                if (v.index[idx] != mBoneNo) {
                    continue;
                }
                //if (v.weight[idx] < 0.5f) {
                //    continue;
                //}

                affected.emplace_back(j);
                affected2.emplace_back(v.pos);
            }
        }
    }

    create(affectedVertices, affectedVertices2);
}


const Bone& OBBCollider::getBone() const {
    return mMesh->getAnimation()->getBone(mBoneNo);
}

void OBBCollider::create(
    const std::vector<std::vector<unsigned>>& vertices,
    const std::vector<std::vector<Vector3>>& vertices2
) {
    const auto& bone = getBone();
    auto boneDefaultPos = getBonePosition(bone);
    auto parentDefaultPos = getBonePosition(*bone.parent);
    auto bonePos = Vector3::lerp(boneDefaultPos, parentDefaultPos, mBoneStart);
    auto parentPos = Vector3::lerp(boneDefaultPos, parentDefaultPos, mBoneEnd);
    auto toParent = (parentPos - bonePos);
    auto center = (bonePos + parentPos) / 2.f;

    Ray ray;
    ray.start = bonePos;
    ray.end = parentPos;

    auto rot = Quaternion::lookRotation(Vector3::normalize(toParent));
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = FLT_MIN;
    float maxY = FLT_MIN;
    float maxZ = FLT_MIN;

    const auto mesh = mMesh->getMesh();
    auto meshCount = mesh->getMeshCount();
    for (unsigned i = 0; i < meshCount; ++i) {
        const auto& verticesPos = mesh->getMeshVerticesPosition(i);
        const auto& affected = vertices[i];
        for (const auto& x : affected) {
            const auto& pos = verticesPos[x];

            test(minX, maxX, pos, center, Vector3::right, rot, ray);
            test(minY, maxY, pos, center, Vector3::up, rot, ray);
            test(minZ, maxZ, pos, center, Vector3::forward, rot, ray);

            test(minX, maxX, pos, ray.start, Vector3::right, rot, ray);
            test(minY, maxY, pos, ray.start, Vector3::up, rot, ray);
            test(minZ, maxZ, pos, ray.start, Vector3::forward, rot, ray);

            test(minX, maxX, pos, ray.end, Vector3::right, rot, ray);
            test(minY, maxY, pos, ray.end, Vector3::up, rot, ray);
            test(minZ, maxZ, pos, ray.end, Vector3::forward, rot, ray);
        }
    }

    if (Math::equal(minX, FLT_MAX)) {
        minX = Math::epsilon;
    }
    if (Math::equal(minY, FLT_MAX)) {
        minY = Math::epsilon;
    }

    //2乗されている距離を正しい距離に直す
    minX = Math::sqrt(minX);
    minY = Math::sqrt(minY);
    minZ = Math::sqrt(minZ);

    mOBB.center = center;
    mOBB.rotation = rot;
    //mOBB.extents = Vector3(maxX, maxY, maxZ);
    mOBB.extents = Vector3(minX, minY, toParent.length() / 2.f);

    mDefaultExtents = mOBB.extents;
}

void OBBCollider::test(
    float& outMin,
    float& outMax,
    const Vector3& target,
    const Vector3& pos,
    const Vector3& axis,
    const Quaternion& rot,
    const Ray& ray
) {
    constexpr float ANGLE1 = 45.f;
    constexpr float ANGLE2 = 180.f - ANGLE1;

    auto toVertPos = Vector3::normalize(target - pos);
    float dot = Vector3::dot(toVertPos, Vector3::transform(axis, rot));
    float angle = Math::acos(dot);
    if (angle < ANGLE1 || ANGLE2 < angle) {
        float distSq = ray.minDistanceSquare(target);
        if (distSq < outMin) {
            outMin = distSq;
        }
        if (distSq > outMax) {
            outMax = distSq;
        }
    }
}

void OBBCollider::beforeComputeWorldMatrix() {
    const auto& bone = getBone();
    auto boneDefaultPos = getBonePosition(bone);
    auto parentDefaultPos = getBonePosition(*bone.parent);
    auto bonePos = Vector3::lerp(boneDefaultPos, parentDefaultPos, mBoneStart);
    auto parentPos = Vector3::lerp(boneDefaultPos, parentDefaultPos, mBoneEnd);
    auto toParent = (parentPos - bonePos);

    mOBB.center = (bonePos + parentPos) / 2.f;
    mOBB.center += mOffsetCenter;
    mOBB.rotation = Quaternion::lookRotation(Vector3::normalize(toParent));

    const auto& t = transform();
    const auto& s = t.getScale();
    mOBB.center = Vector3::transform(mOBB.center, t.getRotation()) * s + t.getPosition();
    mOBB.rotation *= t.getRotation();
    mOBB.extents = mDefaultExtents * s;
}

Vector3 OBBCollider::getBonePosition(const Bone& bone) const {
    const auto& curBones = mAnimation->getBoneCurrentFrameMatrix();
    return Vector3::transform(bone.initMat.getTranslation(), curBones[bone.number]);
}
