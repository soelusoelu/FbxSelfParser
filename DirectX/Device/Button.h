#pragma once

#include "IButton.h"
#include "../Math/Math.h"
#include <functional>
#include <memory>
#include <string>

class Button
    : public IButton
{
public:
    //コールバック関数、位置、寸法
    Button(const std::function<void()>& onClick, const Vector2& pos, const Vector2& dims);
    ~Button();
    virtual void setClickFunc(const std::function<void()>& onClick) override;
    virtual const Vector2& getPosition() const override;
    virtual void setActive(bool value) override;
    virtual bool getActive() const override;
    virtual bool containsPoint(const Vector2& pt) const override;
    virtual bool clickButton(const Vector2& pt) const override;
    virtual void onClick() const override;

private:
    std::function<void()> mOnClick;
    Vector2 mPosition;
    Vector2 mDimensions;
    bool mIsActive;
};

class Sprite;

class SpriteButton
    : public IButton
{
public:
    //コールバック関数、スプライトファイル名、位置
    SpriteButton(const std::function<void()>& onClick, const std::string& fileName, const Vector2& pos);
    ~SpriteButton();
    virtual void setClickFunc(const std::function<void()>& onClick) override;
    virtual const Vector2& getPosition() const override;
    virtual void setActive(bool value) override;
    virtual bool getActive() const override;
    virtual bool containsPoint(const Vector2& pt) const override;
    virtual bool clickButton(const Vector2& pt) const override;
    virtual void onClick() const override;

    void draw(const Matrix4& proj) const;

private:
    std::function<void()> mOnClick;
    std::shared_ptr<Sprite> mSprite;
};
