﻿#pragma once

#include "../System/SystemInclude.h"
#include "../System/Json/JsonObject.h"
#include "../Utility/FileMode.h"
#include <dinput.h>
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")

class Keyboard;
class Mouse;
class JoyPad;

class InputManager {
    InputManager() = delete;
    ~InputManager() = delete;

public:
    //各種入力クラス生成
    static void create();
    //各種入力クラス初期化
    static bool initialize(const HWND& hWnd);
    //ファイルから読み取り
    static void saveAndLoad(JsonObject& inObj, FileMode mode);
    //終了処理
    static void finalize();
    //毎フレーム更新
    static void update();
    //毎フレーム遅延更新
    static void lateUpdate();

    //各種入力クラスにアクセス
    static Keyboard& keyboard();
    static Mouse& mouse();
    static JoyPad& joyPad();

public:
    static inline IDirectInput8* mDirectInput = nullptr;

private:
    static inline Keyboard* mKeyboard = nullptr;
    static inline Mouse* mMouse = nullptr;
    static inline JoyPad* mJoyPad = nullptr;
};
