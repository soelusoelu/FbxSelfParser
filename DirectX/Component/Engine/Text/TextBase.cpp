#include "TextBase.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../System/Window.h"
#include "../../../Utility/JsonHelper.h"

TextBase::TextBase()
    : Component()
    , mPosition(Vector2::zero)
    , mScale(Vector2::one)
    , mColor(ColorPalette::white)
    , mAlpha(1.f)
    , mPivot(Pivot::LEFT_TOP)
    , mIsActive(true)
{
}

TextBase::~TextBase() = default;

void TextBase::onEnable(bool value) {
    setActive(value);
}

void TextBase::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mPosition, "position", inObj, alloc, mode);
    JsonHelper::getSet(mScale, "scale", inObj, alloc, mode);
    JsonHelper::getSet(mColor, "color", inObj, alloc, mode);
    JsonHelper::getSet(mAlpha, "alpha", inObj, alloc, mode);
    JsonHelper::getSet(mIsActive, "isActive", inObj, alloc, mode);

    std::string str;
    if (mode == FileMode::SAVE) {
        PivotFunc::pivotToString(mPivot, str);
        JsonHelper::setString(str, "pivot", inObj, alloc);
    } else {
        if (JsonHelper::getString(str, "pivot", inObj)) {
            PivotFunc::stringToPivot(str, mPivot);
        }
    }
}

void TextBase::drawInspector() {
    ImGuiWrapper::dragVector2("Position", mPosition, 0.1f);
    ImGuiWrapper::dragVector2("Scale", mScale, 0.01f, 0.f, FLT_MAX);
    ImGuiWrapper::colorEdit3("Color", mColor);
    ImGuiWrapper::sliderFloat("Alpha", mAlpha, 0.f, 1.f);
    ImGui::Checkbox("IsActive", &mIsActive);
}

void TextBase::setPosition(const Vector2& pos) {
    mPosition = pos;
}

const Vector2& TextBase::getPosition() const {
    return mPosition;
}

void TextBase::setScale(const Vector2& scale) {
    mScale = scale;
}

const Vector2& TextBase::getScale() const {
    return mScale;
}

void TextBase::setColor(const Vector3& color) {
    mColor = color;
}

const Vector3& TextBase::getColor() const {
    return mColor;
}

void TextBase::setAlpha(float alpha) {
    mAlpha = alpha;
}

float TextBase::getAlpha() const {
    return mAlpha;
}

void TextBase::setPivot(Pivot pivot) {
    mPivot = pivot;
}

Pivot TextBase::getPivot() const {
    return mPivot;
}

void TextBase::setActive(bool value) {
    mIsActive = value;
}

bool TextBase::getActive() const {
    return mIsActive;
}

void TextBase::setDrawString(DrawString* drawString) {
    mDrawString = drawString;
}
