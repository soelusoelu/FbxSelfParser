#pragma once

#include "FbxTimeMode.h"

struct FbxGlobalTime {
    FbxTimeMode timeMode = FbxTimeMode::DEFAULT_MODE;
    long long timeSpanStart = 0ll;
    long long timeSpanStop = 0ll;
};

struct FbxAnimationTime {
    long long localStart = 0ll;
    long long localStop = 0ll;
    long long period = 0ll;
    int startFrame = 0;
    int stopFrame = 0;

    //指定フレームの時間を取得する
    long long getTime(int frame) const {
        return localStart + period * frame;
    }
};
