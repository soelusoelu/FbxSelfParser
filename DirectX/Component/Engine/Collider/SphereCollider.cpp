#include "SphereCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Collision/Collision.h"
#include "../../../Imgui/imgui.h"
#include "../../../Transform/Transform3D.h"

SphereCollider::SphereCollider() :
    Collider(),
    mSphere(),
    mDefaultCenter(Vector3::zero),
    mDefaultRadius(0.f) {
}

SphereCollider::~SphereCollider() = default;

ColliderType SphereCollider::getType() const {
    return ColliderType::SPHERE;
}

void SphereCollider::start() {
    Collider::start();

    auto meshComponent = getComponent<MeshComponent>();
    if (meshComponent) {
        auto mesh = meshComponent->getMesh();
        //メッシュ情報から球を作成する
        SphereHelper::create(mSphere, *mesh);
        mDefaultCenter = mSphere.center;
        mDefaultRadius = mSphere.radius;
    }
}

void SphereCollider::lateUpdate() {
    Collider::lateUpdate();

    if (!mIsAutoUpdate) {
        return;
    }

    auto center = mDefaultCenter + transform().getPosition();
    auto scale = transform().getScale();
    auto maxScaleValue = Math::Max<float>(scale.x, Math::Max<float>(scale.y, scale.z));
    auto radius = mDefaultRadius * maxScaleValue;

    mSphere.center = center;
    mSphere.radius = radius;
}

void SphereCollider::drawInspector() {
    Collider::drawInspector();

    auto& center = mSphere.center;
    float c[3] = { center.x, center.y, center.z };
    ImGui::SliderFloat3("Center", c, FLT_MIN, FLT_MAX);
    ImGui::SliderFloat("Radius", &mSphere.radius, FLT_MIN, FLT_MAX);
}

void SphereCollider::set(const Vector3& center, float radius) {
    mSphere.center = center;
    mSphere.radius = radius;
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Sphere& SphereCollider::getSphere() const {
    return mSphere;
}
