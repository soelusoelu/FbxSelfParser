#pragma once

#include "../Utility/FileMode.h"
#include <rapidjson/document.h>

class Time {
public:
    Time();
    ~Time();
    //保存と読み込み
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);
    //内部時間を進める
    void update();
    //設定を最初の状態に戻す
    void reset();
    //カウントアップタイムがリミットタイムを超えたか
    bool isTime() const;
    //リミットタイムを設定する
    void setLimitTime(float sec);
    //リミットタイムを返す
    float getLimitTime() const;
    //カウントアップタイムの強制設定
    //デバッグ用途にのみ使用
    void setCurrentTime(float sec);
    //現在のカウントアップタイムを返す
    float getCountUpTime() const;
    //現在のカウントダウンタイムを返す
    float getCountDownTime() const;
    //リミットタイムに対してのカウントアップタイムの比率
    float rate() const;
    //強制的にオーバーリミット状態にする
    void forceOverlimit();
    //現在時刻をミリ秒で返す
    static unsigned long long time();

public:
    static inline float deltaTime = 0.f;

private:
    float mCurrentTimer;
    float mLimitTime;
};
