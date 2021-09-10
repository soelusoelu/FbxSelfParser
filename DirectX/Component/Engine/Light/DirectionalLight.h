#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"

class DirectionalLight
    : public Component
{
public:
    DirectionalLight();
    ~DirectionalLight();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    void setDirection(const Vector3& dir);
    const Vector3& getDirection() const;
    void setLightColor(const Vector3& color);
    const Vector3& getLightColor() const;

private:
    Vector3 mDirection;
    Vector3 mLightColor;
};
