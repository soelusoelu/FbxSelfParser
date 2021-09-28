#include "TextNumber.h"
#include "../../../Device/DrawString.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Utility/JsonHelper.h"

TextNumber::TextNumber()
    : TextBase()
    , mNumber(0)
{
}

TextNumber::~TextNumber() = default;

void TextNumber::lateUpdate() {
    if (!mIsActive) {
        return;
    }
    mDrawString->drawNumber(mNumber, mPosition, mScale, mColor, mAlpha, mPivot);
}

void TextNumber::saveAndLoad(JsonObject& inObj, FileMode mode) {
    TextBase::saveAndLoad(inObj, mode);

    JsonHelper::getSet(mNumber, "number", inObj, mode);
}

void TextNumber::drawInspector() {
    TextBase::drawInspector();

    ImGuiWrapper::dragInt("number", mNumber);
}

void TextNumber::setNumber(int number) {
    mNumber = number;
}

int TextNumber::number() const {
    return mNumber;
}
