#include "EngineCamera.h"
#include "SimpleCamera.h"
#include "../../Input/Input.h"

EngineCamera::EngineCamera()
    : mCamera(std::make_unique<SimpleCamera>())
    , mCameraRotation(Quaternion::identity)
    , mLengthCameraToLookAt(0.f)
{
}

EngineCamera::~EngineCamera() = default;

void EngineCamera::initialize() {
    mCameraRotation = Quaternion::identity;
    mLengthCameraToLookAt = 0.f;
}

void EngineCamera::update() {
    const auto& mouse = Input::mouse();

    //1フレームのマウス移動量を取得
    const auto& mouseVelocity = mouse.getMouseVelocity();

    moveCamera(mouse, mouseVelocity);
    rotateLookAtPoint(mouse, mouseVelocity);
    zoomCamera(mouse);
}

SimpleCamera& EngineCamera::getCamera() const {
    return *mCamera;
}

Vector3 EngineCamera::right() const {
    return Vector3::transform(Vector3::right, mCameraRotation);
}

Vector3 EngineCamera::up() const {
    return Vector3::transform(Vector3::up, mCameraRotation);
}

Vector3 EngineCamera::forward() const {
    return Vector3::transform(Vector3::forward, mCameraRotation);
}

void EngineCamera::moveCamera(const IMouse& mouse, const Vector2& mouseVelocity) {
    if (!canMove(mouse, mouseVelocity)) {
        return;
    }

    //カメラの回転をもとに移動量を求める
    auto moveAmount = right() * -mouseVelocity.x;
    moveAmount += up() * mouseVelocity.y;
    //移動速度を決定する
    moveAmount *= MOVE_SPEED;

    //注視点と位置を設定する
    mCamera->lookAt(mCamera->getLookAt() + moveAmount);
    mCamera->setPosition(mCamera->getPosition() + moveAmount);
}

void EngineCamera::rotateLookAtPoint(const IMouse& mouse, const Vector2& mouseVelocity) {
    if (!canRotate(mouse, mouseVelocity)) {
        return;
    }

    //新しい回転軸を計算する
    computeRotation(mouseVelocity);

    //カメラから注視点までの距離を計算する
    computeLengthCameraToLookAt();

    mCamera->setPosition(mCamera->getLookAt() + -forward() * mLengthCameraToLookAt);
}

void EngineCamera::computeRotation(const Vector2& mouseVelocity) {
    auto euler = mCameraRotation.euler();
    euler.y += mouseVelocity.x * ROTATE_SPEED;
    euler.x += mouseVelocity.y * ROTATE_SPEED;
    mCameraRotation.setEuler(euler);
}

void EngineCamera::zoomCamera(const IMouse& mouse) {
    int scroll = mouse.getMouseScrollWheel();
    if (scroll > 0) {
        zoomIn(mouse);
    } else if (scroll < 0) {
        zoomOut(mouse);
    }
}

void EngineCamera::zoomIn(const IMouse& mouse) {
    mCamera->setPosition(mCamera->getPosition() + forward() * ZOOM_SPEED);
}

void EngineCamera::zoomOut(const IMouse& mouse) {
    mCamera->setPosition(mCamera->getPosition() + -forward() * ZOOM_SPEED);
}

Vector3 EngineCamera::getCameraToLookAt() const {
    return (mCamera->getLookAt() - mCamera->getPosition());
}

void EngineCamera::computeLengthCameraToLookAt() {
    mLengthCameraToLookAt = getCameraToLookAt().length();
}

bool EngineCamera::canMove(const IMouse& mouse, const Vector2& mouseVelocity) const {
    if (!mouse.getMouseButton(MouseCode::WheelButton)) {
        return false;
    }
    if (!isMoveMousePosition(mouseVelocity)) {
        return false;
    }

    return true;
}

bool EngineCamera::canRotate(const IMouse& mouse, const Vector2& mouseVelocity) const {
    if (!mouse.getMouseButton(MouseCode::RightButton)) {
        return false;
    }
    if (!isMoveMousePosition(mouseVelocity)) {
        return false;
    }

    return true;
}

bool EngineCamera::isMoveMousePosition(const Vector2& mouseVelocity) const {
    return !Vector2::equal(mouseVelocity, Vector2::zero);
}
