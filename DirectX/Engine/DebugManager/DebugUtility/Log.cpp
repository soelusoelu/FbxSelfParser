#include "Log.h"
#include "../../../Device/DrawString.h"
#include "../../../System/Window.h"
#include "../../../Utility/JsonHelper.h"

Log::Log()
    : FileOperator("Log")
    , mScale(Vector2::one)
    , mNumRowsToDisplay(0)
{
}

Log::~Log() = default;

void Log::initialize() {
    mNumRowsToDisplay = (Window::standardHeight() - Window::height() - DRAW_OFFSET_Y) / (DrawString::HEIGHT * mScale.y);
}

void Log::log(const std::string& message) {
    addLog(message, ColorPalette::white);
}

void Log::logError(const std::string& message) {
    addLog(message, ColorPalette::red);
}

void Log::logWarning(const std::string& message) {
    addLog(message, ColorPalette::yellow);
}

void Log::clear() {
    mLogs.clear();
}

void Log::drawLogs(DrawString& drawString) const {
    const float height = DrawString::HEIGHT * mScale.y;
    auto pos = Vector2(0.f, Window::standardHeight() - height - DRAW_OFFSET_Y);
    for (const auto& log : mLogs) {
        drawString.drawString(log.first, pos, mScale, log.second);
        pos.y -= height;
    }
}

void Log::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mScale, "scale", inObj, alloc, mode);
}

void Log::addLog(const std::string& message, const Vector3& color) {
    mLogs.emplace_back(message, color);
    adjustCapacity();
}

void Log::adjustCapacity() {
    //表示できる量を超えてたら、古いログから削除
    while (mLogs.size() > mNumRowsToDisplay) {
        mLogs.pop_front();
    }
}
