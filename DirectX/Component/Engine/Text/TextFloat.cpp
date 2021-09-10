#include "TextFloat.h"
#include "../../../Device/DrawString.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Utility/JsonHelper.h"

TextFloat::TextFloat()
    : TextBase()
    , mNumber(0.f)
    , mDecimalDigits(1)
{
}

TextFloat::~TextFloat() = default;

void TextFloat::lateUpdate() {
    if (!mIsActive) {
        return;
    }
    mDrawString->drawNumber(mNumber, mPosition, mScale, mDecimalDigits, mColor, mAlpha, mPivot);
}

void TextFloat::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    TextBase::saveAndLoad(inObj, alloc, mode);

    JsonHelper::getSet(mNumber, "number", inObj, alloc, mode);
    JsonHelper::getSet(mDecimalDigits, "decimalDigits", inObj, alloc, mode);
}

void TextFloat::drawInspector() {
    TextBase::drawInspector();

    ImGuiWrapper::dragFloat("number", mNumber);
    ImGuiWrapper::sliderInt("decimalDigits", mDecimalDigits, 0, 8);
}

void TextFloat::setNumber(float number) {
    mNumber = number;
}

float TextFloat::number() const {
    return mNumber;
}

void TextFloat::setDecimalDigits(int decimalDigits) {
    mDecimalDigits = decimalDigits;
}

int TextFloat::getDecimalDigits() const {
    return mDecimalDigits;
}
