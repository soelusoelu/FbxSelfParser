#include "Input.h"
#include "InputManager.h"

IKeyboard& Input::keyboard() {
    return InputManager::keyboard();
}

IMouse& Input::mouse() {
    return InputManager::mouse();
}

JoyPad& Input::joyPad() {
    return InputManager::joyPad();
}
