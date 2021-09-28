#pragma once

#include "IFpsGetter.h"
#include "../SystemInclude.h"
#include "../../Device/FileOperator.h"
#include <memory>

class Time;

class FPSCounter
    : public FileOperator
    , public IFpsGetter
{
public:
    FPSCounter();
    ~FPSCounter();
    FPSCounter(const FPSCounter&) = delete;
    FPSCounter& operator=(const FPSCounter&) = delete;

    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;

    virtual float getFps() const override;

    //設定したフレームレートに固定する
    void fixedFrame();
    //固定フレーム値を設定する
    void setFixedFrame(float fixedFrame);

private:
    //フレームレートを計算する
    void computeFps(float time);

private:
    std::unique_ptr<Time> mDrawUpdateTimer;
    float mFixedFrame;
    float mCurrentFPS;
    LARGE_INTEGER mFrequency;
    unsigned long long mPreviousTime;
};
