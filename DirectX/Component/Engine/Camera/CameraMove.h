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
    CameraMove(const CameraMove&) = delete;
    CameraMove& operator=(const CameraMove&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;
    virtual void drawInspector() override;

private:
    std::shared_ptr<Camera> mCamera;
    float mCameraSpeed;
    float mRotateSpeed;
};
