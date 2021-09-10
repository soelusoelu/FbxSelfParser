#include "Game.h"
#include "AssetsManager.h"
#include "GlobalFunction.h"
#include "SceneManager.h"
#include "Window.h"
#include "FpsCounter/FPSCounter.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "../DirectX/DirectX.h"
#include "../Engine/DebugManager/DebugUtility/LineRenderer/LineRenderer.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Input/InputManager.h"
#include "../Input/Mouse.h"
#include "../Sound/XAudio2/SoundEngine.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/Random.h"

Game::Game()
    : mWindow(nullptr)
    , mFPSCounter(nullptr)
    , mSceneManager(nullptr)
    , mInstance(nullptr)
{
}

Game::~Game() {
    LevelLoader::saveGlobal(this, GLOBAL_DATA_FILE_NAME);

    safeDelete(mSceneManager);

    //imguiの終了処理
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    LineRenderer::finalize();
    Shader::finalize();
    Texture::finalize();
    GameObjectCreater::finalize();
    InputManager::finalize();
    SoundEngine::instance().finalize();
    AssetsManager::instance().finalize();
    MyDirectX::DirectX::instance().finalize();
}

void Game::run(HINSTANCE hInstance) {
    mInstance = hInstance;
    initialize();

    MSG msg = { 0 };
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            mainLoop();
        }
    }
}

void Game::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mWindow->writeAndRead(inObj, alloc, mode);
    mFPSCounter->writeAndRead(inObj, alloc, mode);
    InputManager::saveAndLoad(inObj, alloc, mode);
    mSceneManager->writeAndRead(inObj, alloc, mode);
}

void Game::quit() {
    PostQuitMessage(0);
}

void Game::initialize() {
    mWindow = std::make_unique<Window>();

    mFPSCounter = std::make_unique<FPSCounter>();
    InputManager::create();
    mSceneManager = new SceneManager();

    //ファイルから値を読み込む
    LevelLoader::loadGlobal(this, GLOBAL_DATA_FILE_NAME);

    mWindow->initialize(&InputManager::mouse());
    mWindow->createWindow(mInstance);
    const auto& hwnd = mWindow->gethWnd();

    MyDirectX::DirectX::instance().initialize(hwnd);

    //imguiの初期化
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(MyDirectX::DirectX::instance().device(), MyDirectX::DirectX::instance().deviceContext());

    Random::initialize();
    InputManager::initialize(hwnd);
    GameObjectCreater::initialize();
    mSceneManager->initialize();
}

void Game::mainLoop() {
    //imguiの描画前準備
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    auto& dx = MyDirectX::DirectX::instance();
    dx.clearRenderTarget();
    dx.clearDepthStencilView();

    InputManager::update();
    mWindow->update();

    mSceneManager->update();
    mSceneManager->draw();

    InputManager::lateUpdate();

    //imguiの描画
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    mFPSCounter->fixedFrame();
    dx.present();

    SoundEngine::instance().update();
}
