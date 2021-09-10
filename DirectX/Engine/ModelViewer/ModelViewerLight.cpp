#include "ModelViewerLight.h"
#include "../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../System/Window.h"
#include "../../Utility/JsonHelper.h"

ModelViewerLight::ModelViewerLight()
    : FileOperator("ModelViewerLight")
    , mDirection()
    , mColor(ColorPalette::white)
    , mDirectionDrawPosition()
    , mLengthDirection(0.f)
{
}

ModelViewerLight::~ModelViewerLight() = default;

void ModelViewerLight::drawGUI() {
    ImGui::Text("DirectionalLight");

    auto euler = mDirection.euler();
    if (ImGuiWrapper::dragVector3("Direction", euler, 0.1f)) {
        mDirection.setEuler(euler);
    }
    ImGuiWrapper::colorEdit3("Color", mColor);
}

Vector3 ModelViewerLight::getDirection() const {
    return Vector3::transform(Vector3::up, mDirection);
}

const Vector3& ModelViewerLight::getColor() const {
    return mColor;
}

void ModelViewerLight::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mDirection, "direction", inObj, alloc, mode);
    JsonHelper::getSet(mColor, "color", inObj, alloc, mode);
    JsonHelper::getSet(mDirectionDrawPosition, "directionDrawPosition", inObj, alloc, mode);
    JsonHelper::getSet(mLengthDirection, "lengthDirection", inObj, alloc, mode);
}
