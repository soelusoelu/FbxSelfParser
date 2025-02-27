﻿#include "Mouse.h"
#include "InputUtility.h"
#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
#include "../System/Window.h"

Mouse::Mouse()
    : mMouseDevice(nullptr)
    , mCurrentMouseState()
    , mPreviousMouseState()
    , mhWnd(nullptr)
    , mCurrentMousePosition(Vector2::zero)
    , mPreviousMousePosition(Vector2::zero)
    , mMouseVelocity(Vector2::zero)
    , mScrollWheel(0)
{
}

Mouse::~Mouse() {
    safeRelease<IDirectInputDevice8>(mMouseDevice);
}

bool Mouse::getMouseButtonDown(MouseCode button) const {
    return (mCurrentMouseState.rgbButtons[static_cast<int>(button)] & InputUtility::VERSION && !(mPreviousMouseState.rgbButtons[static_cast<int>(button)] & InputUtility::VERSION));
}

bool Mouse::getMouseButton(MouseCode button) const {
    return (mCurrentMouseState.rgbButtons[static_cast<int>(button)] & InputUtility::VERSION && (mPreviousMouseState.rgbButtons[static_cast<int>(button)] & InputUtility::VERSION));
}

bool Mouse::getMouseButtonUp(MouseCode button) const {
    return (!(mCurrentMouseState.rgbButtons[static_cast<int>(button)] & InputUtility::VERSION) && mPreviousMouseState.rgbButtons[static_cast<int>(button)] & InputUtility::VERSION);
}

const Vector2& Mouse::getMousePosition() const {
    return mCurrentMousePosition;
}

const Vector2& Mouse::getMouseVelocity() const {
    return mMouseVelocity;
}

int Mouse::getMouseScrollWheel() const {
    return mScrollWheel;
}

void Mouse::setWheelScrollValue(int value) {
    mScrollWheel = value;
}

bool Mouse::initialize(const HWND& hWnd, IDirectInput8* directInput) {
    mhWnd = hWnd;

    //「DirectInputデバイス」オブジェクトの作成
    if (FAILED(directInput->CreateDevice(GUID_SysMouse, &mMouseDevice, NULL))) {
        return false;
    }
    //デバイスをマウスに設定
    if (FAILED(mMouseDevice->SetDataFormat(&c_dfDIMouse2))) {
        return false;
    }
    //協調レベルの設定
    if (FAILED(mMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
        return false;
    }
    //デバイスを「取得」する
    mMouseDevice->Acquire();

    return true;
}

void Mouse::update() {
    mPreviousMouseState = mCurrentMouseState;
    mPreviousMousePosition = mCurrentMousePosition;

    HRESULT hr = mMouseDevice->Acquire();
    if ((hr == DI_OK) || (hr == S_FALSE)) {
        mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mCurrentMouseState);
    }

    updateMousePosition();
    clampMousePosition();

    mMouseVelocity = mCurrentMousePosition - mPreviousMousePosition;
}

void Mouse::lateUpdate() {
    mScrollWheel = 0;
}

void Mouse::stringToJoyCode(const std::string& src, MouseCode* dst) {
    auto key = MouseCode::None;

    if (src == "LeftButton") {
        key = MouseCode::LeftButton;
    } else if (src == "RightButton") {
        key = MouseCode::RightButton;
    } else if (src == "WheelButton") {
        key = MouseCode::WheelButton;
    } else if (src == "SideButton1") {
        key = MouseCode::SideButton1;
    } else if (src == "SideButton2") {
        key = MouseCode::SideButton2;
    } else if (src == "SideButton3") {
        key = MouseCode::SideButton3;
    }

    if (key != MouseCode::None) {
        *dst = key;
    }
}

void Mouse::updateMousePosition() {
    POINT point = { 0 };
    //カーソル位置の絶対座標を取得
    GetCursorPos(&point);
    //ウィンドウ内の座標に変換
    ScreenToClient(mhWnd, &point);
    mCurrentMousePosition.x = static_cast<float>(point.x);
    mCurrentMousePosition.y = static_cast<float>(point.y);
    mCurrentMousePosition.x *= Window::windowToClientSize().x;
    mCurrentMousePosition.y *= Window::windowToClientSize().y;
}

void Mouse::clampMousePosition() {
    mCurrentMousePosition.clamp(
        Vector2::zero,
        Vector2(static_cast<float>(Window::standardWidth()), static_cast<float>(Window::standardHeight()))
    );
}
