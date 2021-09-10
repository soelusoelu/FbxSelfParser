#pragma once

#include "../Math/Math.h"
#include <vector>

struct AABB;

//AABBを生成するクラス
class AABBCreater {
public:
    //頂点配列からAABBを作成する
    static AABB create(const std::vector<Vector3>& positions);

private:
    AABBCreater() = delete;
    ~AABBCreater() = delete;
    AABBCreater(const AABBCreater&) = delete;
    AABBCreater& operator=(const AABBCreater&) = delete;
};
