#include "Text.h"
#include "../../../Device/DrawString.h"
#include "../../../Imgui/imgui.h"
#include "../../../Utility/JsonHelper.h"
#include "../../../Utility/StringUtil.h"

Text::Text()
    : TextBase()
    , mText("")
{
}

Text::~Text() = default;

void Text::lateUpdate() {
    TextBase::lateUpdate();

    if (!mIsActive) {
        return;
    }

    auto split = StringUtil::split(mText, '\n');
    auto pos = mPosition;

    for (const auto& s : split) {
        mDrawString->drawString(s, pos, mScale, mColor, mAlpha, mPivot);
        pos.y += DrawString::HEIGHT * mScale.y;
    }
}

void Text::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    TextBase::saveAndLoad(inObj, alloc, mode);

    JsonHelper::getSet(mText, "text", inObj, alloc, mode);
}

void Text::drawInspector() {
    TextBase::drawInspector();

    ImGui::Text("Text: %s", mText.c_str());
}

void Text::setText(const std::string& text) {
    mText = text;
}

const std::string& Text::text() const {
    return mText;
}
