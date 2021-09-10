#include "InputManager.h"
#include "JoyPad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "../System/GlobalFunction.h"

void InputManager::create() {
    mKeyboard = new Keyboard();
    mMouse = new Mouse();
    mJoyPad = new JoyPad();
}

bool InputManager::initialize(const HWND& hWnd) {
    //「DirectInput」オブジェクトの作成
    if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr))) {
        return false;
    }

    mKeyboard->initialize(hWnd, mDirectInput);
    mMouse->initialize(hWnd, mDirectInput);
    mJoyPad->initialize(hWnd, mDirectInput);

    return true;
}

void InputManager::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mKeyboard->writeAndRead(inObj, alloc, mode);
    mJoyPad->writeAndRead(inObj, alloc, mode);
}

void InputManager::finalize() {
    safeRelease(mDirectInput);
    safeDelete(mKeyboard);
    safeDelete(mMouse);
    safeDelete(mJoyPad);
}

void InputManager::update() {
    mKeyboard->update();
    mMouse->update();
    mJoyPad->update();
}

void InputManager::lateUpdate() {
    mMouse->lateUpdate();
}

Keyboard& InputManager::keyboard() {
    return *mKeyboard;
}

Mouse& InputManager::mouse() {
    return *mMouse;
}

JoyPad& InputManager::joyPad() {
    return *mJoyPad;
}
