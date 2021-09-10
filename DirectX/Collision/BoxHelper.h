#pragma once

#include "BoxConstantGroup.h"
#include "../Math/Math.h"

struct OBB;

class BoxHelper {
private:
    BoxHelper() = delete;
    ~BoxHelper() = delete;

public:
    //OBBの頂点配列を取得する
    static BoxPoints getPoints(const OBB& obb);
};
