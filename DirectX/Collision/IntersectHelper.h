#pragma once

#include "../Math/Math.h"

class GameObject;
struct RaycastHit;
struct Triangle;

namespace IntersectHelper {
//RaycastHitを更新する
void updateRaycastHit(
    RaycastHit& raycastHit,
    GameObject* hitObject,
    float distanceSquare,
    const Vector3& point,
    const Triangle& polygon
);

//RaycastHitを更新する
void updateRaycastHit(
    RaycastHit& target,
    const RaycastHit& newRaycastHit
);
};
