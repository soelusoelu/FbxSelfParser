#include "Transform3D.h"
#include "ParentChildRelationship.h"
#include "../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../GameObject/GameObject.h"
#include "../Imgui/imgui.h"
#include "../Utility/JsonHelper.h"

Transform3D::Transform3D(GameObject* gameObject)
    : mGameObject(gameObject)
    , mParentChildRelation(std::make_unique<ParentChildRelationship>(this))
    , mLocalTransform(Matrix4::identity)
    , mWorldTransform(Matrix4::identity)
    , mPosition(Vector3::zero)
    , mRotation(Quaternion::identity)
    , mPivot(Vector3::zero)
    , mScale(Vector3::one)
{
}

Transform3D::~Transform3D() = default;

void Transform3D::computeMatrix() {
    //親がいる場合は親に任せる
    if (mParentChildRelation->parent()) {
        return;
    }

    //ローカル行列を計算する
    computeLocalMatrix();
    //ワールド行列を計算する
    computeWorldMatrix();

    //子のワールド行列を計算する
    computeChildrenTransform();
}

const Matrix4& Transform3D::getLocalTransform() const {
    return mLocalTransform;
}

const Matrix4& Transform3D::getWorldTransform() const {
    return mWorldTransform;
}

void Transform3D::setPosition(const Vector3& pos) {
    mPosition = pos;
}

Vector3 Transform3D::getPosition() const {
    return mWorldTransform.getTranslation();
}

const Vector3& Transform3D::getLocalPosition() const {
    return mPosition;
}

void Transform3D::translate(const Vector3& translation) {
    mPosition += translation;
}

void Transform3D::translate(float x, float y, float z) {
    mPosition.x += x;
    mPosition.y += y;
    mPosition.z += z;
}

void Transform3D::setRotation(const Quaternion& rot) {
    mRotation = rot;
}

void Transform3D::setRotation(const Vector3& axis, float angle) {
    angle *= 0.5f;
    auto sinAngle = Math::sin(angle);

    mRotation.x = axis.x * sinAngle;
    mRotation.y = axis.y * sinAngle;
    mRotation.z = axis.z * sinAngle;
    mRotation.w = Math::cos(angle);
}

void Transform3D::setRotation(const Vector3& eulers) {
    mRotation.setEuler(eulers);
}

Quaternion Transform3D::getRotation() const {
    auto rotation = mRotation;

    const auto ep = mParentChildRelation->getEquipmentPart();
    if (ep) {
        rotation *= ep->getQuaternion();
    }

    auto parent = mParentChildRelation->parent();
    while (parent) {
        rotation = parent->transform().mRotation;
        parent = parent->parent();
    }

    return rotation;
}

const Quaternion& Transform3D::getLocalRotation() const {
    return mRotation;
}

void Transform3D::rotate(const Vector3& axis, float angle) {
    angle *= 0.5f;
    float sinAngle = Math::sin(angle);

    Quaternion inc(
        axis.x * sinAngle,
        axis.y * sinAngle,
        axis.z * sinAngle,
        Math::cos(angle)
    );

    mRotation *= inc;
}

void Transform3D::rotate(const Vector3& eulers) {
    rotate(Vector3::forward, eulers.z);
    rotate(Vector3::right, eulers.x);
    rotate(Vector3::up, eulers.y);
}

void Transform3D::lookAt(const Transform3D& target, const Vector3& upwards) {
    auto forward = Vector3::normalize(target.getPosition() - getPosition());
    setRotation(Quaternion::lookRotation(forward, upwards));
}

void Transform3D::setPivot(const Vector3& pivot) {
    mPivot = pivot;
}

const Vector3& Transform3D::getPivot() const {
    return mPivot;
}

void Transform3D::setScale(const Vector3& scale) {
    mScale = scale;
}

void Transform3D::setScale(float scale) {
    mScale.x = scale;
    mScale.y = scale;
    mScale.z = scale;
}

Vector3 Transform3D::getScale() const {
    return mWorldTransform.getScale();
}

const Vector3& Transform3D::getLocalScale() const {
    return mScale;
}

Vector3 Transform3D::forward() const {
    return Vector3::transform(Vector3::forward, mRotation);
}

Vector3 Transform3D::up() const {
    return Vector3::transform(Vector3::up, mRotation);
}

Vector3 Transform3D::right() const {
    return Vector3::transform(Vector3::right, mRotation);
}

GameObject& Transform3D::gameObject() const {
    return *mGameObject;
}

ParentChildRelationship& Transform3D::getParentChildRelation() const {
    return *mParentChildRelation;
}

void Transform3D::callbackBeforeComputeWorldMatrix(const std::function<void()>& f) {
    mCallbackBeforeComputeWorldMatrix += f;
}

void Transform3D::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mPosition, "position", inObj, alloc, mode);
    JsonHelper::getSet(mRotation, "rotation", inObj, alloc, mode);
    JsonHelper::getSet(mScale, "scale", inObj, alloc, mode);
    JsonHelper::getSet(mPivot, "pivot", inObj, alloc, mode);

    if (mode == FileMode::LOAD) {
        computeLocalMatrix();
        computeWorldMatrix();
    }
}

void Transform3D::drawInspector() {
    ImGuiWrapper::dragVector3("Position", mPosition, 0.01f);

    auto euler = mRotation.euler();
    if (ImGuiWrapper::dragVector3("Rotation", euler, 0.1f)) {
        mRotation.setEuler(euler);
    }

    ImGuiWrapper::dragVector3("Scale", mScale, 0.01f);
}

void Transform3D::computeLocalMatrix() {
    mLocalTransform = Matrix4::createTranslation(-mPivot); //ピボットを原点に
    mLocalTransform *= Matrix4::createScale(mScale);
    mLocalTransform *= Matrix4::createFromQuaternion(mRotation);
    mLocalTransform *= Matrix4::createTranslation(mPosition);
}

void Transform3D::computeWorldMatrix() {
    mWorldTransform = mLocalTransform;

    //ワールド行列計算前呼び出し
    mCallbackBeforeComputeWorldMatrix();

    const auto ep = mParentChildRelation->getEquipmentPart();
    if (ep) {
        mWorldTransform *= *ep;
    }

    auto parent = mParentChildRelation->parent();
    while (parent) {
        mWorldTransform *= parent->transform().mLocalTransform;
        parent = parent->parent();
    }
}

void Transform3D::computeChildrenTransform() {
    const auto& children = mParentChildRelation->getChildren();
    for (const auto& child : children) {
        auto& childTransform = child->transform();
        //子のローカル行列を計算する
        childTransform.computeLocalMatrix();

        //さらに子へ降りていく
        child->transform().computeChildrenTransform();
    }

    //ワールド行列を計算する
    computeWorldMatrix();
}
