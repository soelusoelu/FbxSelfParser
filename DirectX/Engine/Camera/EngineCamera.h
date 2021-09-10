#pragma once

#include "../../Input/IMouse.h"
#include "../../Math/Math.h"
#include <memory>

class SimpleCamera;

//エンジンのデフォルトで使われるカメラ
class EngineCamera {
public:
    EngineCamera();
    ~EngineCamera();
    void initialize();
    void update();
    SimpleCamera& getCamera() const;
    //回転を考慮した右ベクトルを取得する
    Vector3 right() const;
    //回転を考慮した上ベクトルを取得する
    Vector3 up() const;
    //回転を考慮した前方ベクトルを取得する
    Vector3 forward() const;

private:
    EngineCamera(const EngineCamera&) = delete;
    EngineCamera& operator=(const EngineCamera&) = delete;

    //カメラを移動する
    void moveCamera(const IMouse& mouse, const Vector2& mouseVelocity);
    //注視点を中心にカメラを回転する
    void rotateLookAtPoint(const IMouse& mouse, const Vector2& mouseVelocity);
    //マウスの移動量から回転軸を計算する
    void computeRotation(const Vector2& mouseVelocity);
    //ズームイン、ズームアウトをする
    void zoomCamera(const IMouse& mouse);
    //ズームイン
    void zoomIn(const IMouse& mouse);
    //ズームアウト
    void zoomOut(const IMouse& mouse);

    //カメラから注視点までのベクトルを求める
    Vector3 getCameraToLookAt() const;
    //カメラから注視点までの距離を計算する
    void computeLengthCameraToLookAt();

    //カメラを移動できる状態か
    bool canMove(const IMouse& mouse, const Vector2& mouseVelocity) const;
    //カメラを回転できる状態か
    bool canRotate(const IMouse& mouse, const Vector2& mouseVelocity) const;
    //マウスが移動したか
    bool isMoveMousePosition(const Vector2& mouseVelocity) const;

private:
    std::unique_ptr<SimpleCamera> mCamera;
    Quaternion mCameraRotation;
    float mLengthCameraToLookAt;

    static constexpr float MOVE_SPEED = 0.0065f;
    static constexpr float ROTATE_SPEED = 0.5f;
    static constexpr float ZOOM_SPEED = 1.f;
};
