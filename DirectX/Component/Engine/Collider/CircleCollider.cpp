﻿#include "CircleCollider.h"
#include "../Sprite/SpriteComponent.h"
#include "../../../Imgui/imgui.h"
#include "../../../Transform/Transform2D.h"

CircleCollider::CircleCollider() :
    Collider(),
    mSprite(nullptr),
    mCircle() {
}

CircleCollider::~CircleCollider() = default;

ColliderType CircleCollider::getType() const {
    return ColliderType::CIRCLE;
}

void CircleCollider::start() {
    Collider::start();

    mSprite = getComponent<SpriteComponent>();
    circleUpdate();
}

void CircleCollider::update() {
    Collider::update();

    if (!mIsAutoUpdate) {
        return;
    }

    circleUpdate();
}

void CircleCollider::drawInspector() {
    Collider::drawInspector();

    auto& center = mCircle.center;
    float c[2] = { center.x, center.y };
    ImGui::SliderFloat2("Center", c, FLT_MIN, FLT_MAX);
    ImGui::SliderFloat("Radius", &mCircle.radius, FLT_MIN, FLT_MAX);
}

void CircleCollider::set(const Vector2& center, float radius) {
    mCircle.center = center;
    mCircle.radius = radius;
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const Circle& CircleCollider::getCircle() const {
    return mCircle;
}

void CircleCollider::circleUpdate() {
    if (!mSprite) {
        return;
    }

    const auto& t = mSprite->transform();
    auto maxSize = Math::Max(mSprite->getWidth(), mSprite->getHeight());
    const auto& scale = t.getScale();
    auto maxScale = Math::Max(scale.x, scale.y);
    auto radius = maxSize / 2.f * maxScale;

    mCircle.center = t.getPosition();
    mCircle.radius = radius;
}
