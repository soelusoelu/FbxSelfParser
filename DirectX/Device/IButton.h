#pragma once

#include <functional>

class Vector2;

class IButton {
public:
    virtual ~IButton() = default;
    //ボタンの機能を設定
    virtual void setClickFunc(const std::function<void()>& onClick) = 0;
    //ボタンの位置を返す
    virtual const Vector2& getPosition() const = 0;
    //アクティブ指定
    virtual void setActive(bool value) = 0;
    //アクティブか
    virtual bool getActive() const = 0;
    //ptがボタンの範囲内ならtrueを返す
    virtual bool containsPoint(const Vector2& pt) const = 0;
    //ボタンがクリックされたら
    virtual bool clickButton(const Vector2& pt) const = 0;
    //ボタンが押されたときに呼び出される
    virtual void onClick() const = 0;
};
