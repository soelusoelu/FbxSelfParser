#pragma once

class IMouseWheelScrollValueSetter {
public:
    virtual ~IMouseWheelScrollValueSetter() = default;
    //ホイールのスクロールに応じて値を設定する
    virtual void setWheelScrollValue(int value) = 0;
};
