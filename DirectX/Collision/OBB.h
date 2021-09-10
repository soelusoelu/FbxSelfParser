#pragma once

#include "../Math/Math.h"

struct OBB {
    Vector3 center;
    Quaternion rotation;
    Vector3 extents;
};
