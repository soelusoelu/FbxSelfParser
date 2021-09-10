#include "FPSCounter.h"
#include "../../Device/Time.h"
#include "../../Utility/JsonHelper.h"

FPSCounter::FPSCounter()
    : FileOperator("FPSCounter")
    , mDrawUpdateTimer(std::make_unique<Time>())
    , mFixedFrame(60.f)
    , mCurrentFPS(60.f)
    , mFrequency()
    , mPreviousTime()
{
    mDrawUpdateTimer->setLimitTime(0.5f);
}

FPSCounter::~FPSCounter() = default;

float FPSCounter::getFps() const {
    return mCurrentFPS;
}

void FPSCounter::fixedFrame() {
    float time = 0.f;
    float fix = 1000.f / mFixedFrame;

    while (time < fix) {
        QueryPerformanceFrequency(&mFrequency);

        time = static_cast<float>(Time::time() - mPreviousTime);
        time *= 1000.f / static_cast<float>(mFrequency.QuadPart);
    }
    mPreviousTime = Time::time();

    float deltaTime = static_cast<float>(time / 1000.f);
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }
    Time::deltaTime = deltaTime;

#ifdef _DEBUG
    computeFps(time);
#endif // _DEBUG
}

void FPSCounter::setFixedFrame(float fixedFrame) {
    mFixedFrame = fixedFrame;
}

void FPSCounter::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mFixedFrame, "fps", inObj, alloc, mode);
}

void FPSCounter::computeFps(float time) {
    mDrawUpdateTimer->update();
    if (mDrawUpdateTimer->isTime()) {
        mDrawUpdateTimer->reset();
        mCurrentFPS = 1000.f / time;
    }
}
