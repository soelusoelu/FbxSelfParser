#pragma once

#include "FbxTimeMode.h"

struct FbxAnimationTime {
    FbxTimeMode timeMode = FbxTimeMode::DEFAULT_MODE;
    long long timeSpanStart = 0;
    long long timeSpanStop = 0;
    long long start = 0;
    long long stop = 0;
    long long period = 0;
    int startFrame = 0;
    int stopFrame = 0;

    //指定フレームの時間を取得する
    long long getTime(int frame) const {
        return start + period * frame;
    }
};
