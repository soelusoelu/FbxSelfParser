#pragma once

#include "../Math/Math.h"
#include <vector>

struct OBB;

//OBBを生成するクラス
class OBBCreater {
private:
    OBBCreater() = delete;
    ~OBBCreater() = delete;

public:
    static OBB create(const std::vector<Vector3>& positions);

private:
    //行列を収集する
    static Matrix3 collectMatrix(const std::vector<Vector3>& positions);
    //固有ベクトルを求める
    static Matrix3 computeEigenvector(const Matrix3& mat);
    //行列の各成分の絶対値の最大値とその位置を求める
    static void getMaxValueForMatrix(const Matrix3& mat, float& outMax, unsigned& outRow, unsigned& outColumn);
};
