#include "DirectionalLight.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Imgui/imgui.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

DirectionalLight::DirectionalLight() :
    Component(),
    mDirection(Vector3::down),
    mLightColor(Vector3::one) {
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::start() {
    transform().rotate(mDirection);
}

void DirectionalLight::lateUpdate() {
    //メッシュの向きと合わせる
    mDirection = Vector3::transform(Vector3::down, transform().getRotation());
}

void DirectionalLight::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mDirection, "direction", inObj, alloc, mode);
    JsonHelper::getSet(mLightColor, "color", inObj, alloc, mode);
}

void DirectionalLight::drawInspector() {
    ImGuiWrapper::colorEdit3("Color", mLightColor);
}

void DirectionalLight::setDirection(const Vector3& dir) {
    mDirection = dir;
}

const Vector3& DirectionalLight::getDirection() const {
    return mDirection;
}

void DirectionalLight::setLightColor(const Vector3& color) {
    mLightColor = color;
}

const Vector3& DirectionalLight::getLightColor() const {
    return mLightColor;
}
