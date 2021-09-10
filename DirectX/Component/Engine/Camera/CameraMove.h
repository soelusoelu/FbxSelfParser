#pragma once

#include "../../Component.h"
#include <memory>

class Camera;

//カメラの動きを扱うクラス
class CameraMove
    : public Component
{
public:
    CameraMove();
    ~CameraMove();
    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

private:
    CameraMove(const CameraMove&) = delete;
    CameraMove& operator=(const CameraMove&) = delete;

private:
    std::shared_ptr<Camera> mCamera;
    float mCameraSpeed;
    float mRotateSpeed;
};
