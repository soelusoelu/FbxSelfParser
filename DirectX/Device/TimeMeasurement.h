#pragma once

#include <chrono>
#include <vector>

//時間計測クラス
class TimeMeasurement {
public:
    TimeMeasurement(int numMeasurements = 10);
    ~TimeMeasurement();
    //時間計測開始
    void start();
    //時間計測終了
    //ミリ秒で実行時間を返す
    double end();

private:
    TimeMeasurement(const TimeMeasurement&) = delete;
    TimeMeasurement& operator=(const TimeMeasurement&) = delete;

    //過去に計測した時間から平均を求める
    double calcAverage() const;

private:
    //計測開始時の時間
    std::chrono::system_clock::time_point mStart;
    //過去に計測した時間
    std::vector<double> mMeasurements;
    //計測した回数
    int mCount;
    //何回計測するか
    int mNumMeasurements;
};
