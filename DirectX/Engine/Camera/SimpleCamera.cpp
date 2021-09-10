#include "SimpleCamera.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../System/Window.h"

SimpleCamera::SimpleCamera()
    : mView()
    , mProjection()
    , mPosition()
    , mLookAt(Vector3::forward * 10.f)
    , mUp(Vector3::up)
    , mFOV(45.f)
    , mNearClip(0.1f)
    , mFarClip(1000.f)
{
    calcView();
    calcProjection();
}

SimpleCamera::~SimpleCamera() = default;

void SimpleCamera::setView(const Matrix4& view) {
    mView = view;
}

const Matrix4& SimpleCamera::getView() const {
    return mView;
}

void SimpleCamera::setProjection(const Matrix4& projection) {
    mProjection = projection;
}

const Matrix4& SimpleCamera::getProjection() const {
    return mProjection;
}

Matrix4 SimpleCamera::getViewProjection() const {
    return mView * mProjection;
}

void SimpleCamera::setPosition(const Vector3& pos) {
    mPosition = pos;
    calcView();
}

const Vector3& SimpleCamera::getPosition() const {
    return mPosition;
}

void SimpleCamera::lookAt(const Vector3& position) {
    mLookAt = position;
    calcView();
}

const Vector3& SimpleCamera::getLookAt() const {
    return mLookAt;
}

void SimpleCamera::setFov(float fov) {
    mFOV = fov;
    calcProjection();
}

float SimpleCamera::getFov() const {
    return mFOV;
}

void SimpleCamera::setNearClip(float nearClip) {
    mNearClip = nearClip;
    calcProjection();
}

float SimpleCamera::getNearClip() const {
    return mNearClip;
}

void SimpleCamera::setFarClip(float farClip) {
    mFarClip = farClip;
    calcProjection();
}

float SimpleCamera::getFarClip() const {
    return mFarClip;
}

Vector3 SimpleCamera::screenToWorldPoint(const Vector2& position, float z) const {
    return CameraHelper::screenToWorldPoint(position, mView, mProjection, z);
}

Ray SimpleCamera::screenToRay(const Vector2& position, float z) const {
    return CameraHelper::screenToRay(mPosition, position, mView, mProjection, z);
}

void SimpleCamera::calcView() {
    mView = Matrix4::createLookAt(mPosition, mLookAt, mUp);
}

void SimpleCamera::calcProjection() {
    mProjection = Matrix4::createPerspectiveFOV(Window::width(), Window::height(), mFOV, mNearClip, mFarClip);
}
