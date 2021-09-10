#pragma once

#include "SystemInclude.h"
#include "../Device/FileOperator.h"
#include "../Input/IMouseWheelScrollValueSetter.h"
#include "../Math/Math.h"
#include <string>

class Window
    : public FileOperator
{
public:
    Window();
    ~Window();
    void initialize(IMouseWheelScrollValueSetter* setter);
    void createWindow(HINSTANCE hInstance);
    LRESULT msgProc(HWND, UINT, WPARAM, LPARAM);
    HWND gethWnd() const;
    void update();
    //ゲームウィンドウの横幅(px)
    static int width();
    //ゲームウィンドウの縦幅(px)
    static int height();
    //ゲームウィンドウのサイズ(width * height)
    static Vector2 size();
    static int standardWidth();
    static int standardHeight();
    static Vector2 windowToClientSize();
    //ゲームウィンドウをフルHDサイズに変える補正値を取得する
    static Vector2 getWindowCorrect();

private:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    void updateWindowToClientSize();

private:
    HWND mhWnd;
    WNDCLASSEX mWndClass; //ウィンドウクラスの設定
    IMouseWheelScrollValueSetter* mMouseWheelScrollValueSetter;
    std::string mTitle;
    static inline int mGameWidth = 1080;
    static inline int mGameHeight = 720;
    static inline int mStandardWidth = 1920;
    static inline int mStandardHeight = 1080;
    static inline Vector2 mWindowToClientSize = Vector2::zero;
};

