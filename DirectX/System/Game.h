﻿#pragma once

#include "SystemInclude.h"
#include "Json/JsonObject.h"
#include "../Utility/FileMode.h"
#include <memory>

class Window;
class FPSCounter;
class SceneManager;

class Game {
public:
    Game();
    ~Game();
    //アプリケーションの実行
    void run(HINSTANCE hInstance);
    //グローバルファイルを読み書きする
    void saveAndLoad(JsonObject& inObj, FileMode mode);

    //アプリケーションの終了
    static void quit();

private:
    //各クラスの初期化
    void initialize();
    //メインループ
    void mainLoop();
    //グローバルファイルを読み込む
    void loadGlobalFile();
    //グローバルファイルを書き込む
    void saveGlobalFile();

private:
    std::unique_ptr<Window> mWindow;
    std::unique_ptr<FPSCounter> mFPSCounter;
    std::shared_ptr<JsonObject> mRootObject;
    SceneManager* mSceneManager;
    HINSTANCE mInstance;

    static inline const char* GLOBAL_DATA_FILE_NAME = "Global.json";
};
