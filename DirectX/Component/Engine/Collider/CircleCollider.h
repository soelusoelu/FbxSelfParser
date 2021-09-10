#pragma once

#include "Collider.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include <memory>

class SpriteComponent;

class CircleCollider
    : public Collider
{
public:
    CircleCollider();
    ~CircleCollider();
    virtual ColliderType getType() const override;

    virtual void start() override;
    virtual void update() override;
    virtual void drawInspector() override;

    void set(const Vector2& center, float radius);
    const Circle& getCircle() const;

private:
    void circleUpdate();

private:
    Circle mCircle;
    std::shared_ptr<SpriteComponent> mSprite;
};
