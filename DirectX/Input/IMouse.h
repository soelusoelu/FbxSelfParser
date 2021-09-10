#pragma once

class Vector2;

enum class MouseCode {
    LeftButton = 0,
    RightButton = 1,
    WheelButton = 2,
    SideButton1 = 3,
    SideButton2 = 4,
    SideButton3 = 5,

    None = 512
};

class IMouse {
public:
    virtual ~IMouse() = default;
    //キーが押された瞬間か
    virtual bool getMouseButtonDown(MouseCode button) const = 0;
    //キーが押され続けているか
    virtual bool getMouseButton(MouseCode button) const = 0;
    //キーが離れた瞬間か
    virtual bool getMouseButtonUp(MouseCode button) const = 0;
    //マウスのウィンドウ位置を取得する
    virtual const Vector2& getMousePosition() const = 0;
    //1フレームにおけるマウスの移動量を取得する
    virtual const Vector2& getMouseVelocity() const = 0;
    //ホイール回転量を取得する
    virtual int getMouseScrollWheel() const = 0;
};
