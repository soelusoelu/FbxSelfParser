#pragma once

#include "../Math/Math.h"
#include "Triangle.h"

class GameObject;

struct RaycastHit {
    GameObject* hitObject;
    bool isHit;
    float distanceSquare;
    Vector3 point;
    Triangle polygon;

    RaycastHit()
        : hitObject(nullptr)
        , isHit(false)
        , distanceSquare(FLT_MAX)
        , point()
        , polygon() {
    }
};
