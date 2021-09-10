#pragma once

#include "Collider.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IMesh.h"
#include "../../../Mesh/IMeshLoader.h"

class SphereCollider
    : public Collider
{
public:
    SphereCollider();
    ~SphereCollider();
    virtual ColliderType getType() const override;

    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void drawInspector() override;

    //中心位置と半径を直接設定する
    void set(const Vector3& center, float radius);
    //球を取得する
    const Sphere& getSphere() const;

private:
    Sphere mSphere;
    Vector3 mDefaultCenter;
    float mDefaultRadius;
};
