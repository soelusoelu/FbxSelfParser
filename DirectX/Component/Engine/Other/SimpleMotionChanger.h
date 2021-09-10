#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;

class SimpleMotionChanger
    : public Component
{
public:
    SimpleMotionChanger();
    ~SimpleMotionChanger();
    virtual void start() override;
    virtual void update() override;

private:
    SimpleMotionChanger(const SimpleMotionChanger&) = delete;
    SimpleMotionChanger& operator=(const SimpleMotionChanger&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
};
