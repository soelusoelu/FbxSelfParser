#pragma once

#include "../../../Device/FileOperator.h"
#include "../../../Math/Math.h"
#include <list>
#include <string>
#include <utility>

class DrawString;

class Log
    : public FileOperator
{
public:
    Log();
    ~Log();
    void initialize();
    void log(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void clear();
    void drawLogs(DrawString& drawString) const;

private:
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    void addLog(const std::string& message, const Vector3& color);
    void adjustCapacity();

private:
    //表示する文字列と色
    std::list<std::pair<std::string, Vector3>> mLogs;
    //文字のスケール
    Vector2 mScale;
    //画面に表示するログの行数
    int mNumRowsToDisplay;

    static constexpr float DRAW_OFFSET_Y = 32.f;
};
