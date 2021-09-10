#include "Button.h"
#include "../Collision/Collision.h"
#include "../Input/Input.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../Sprite/SpriteUtility.h"
#include "../Transform/Transform2D.h"

Button::Button(const std::function<void()>& onClick, const Vector2& pos, const Vector2& dims)
    : mOnClick(onClick)
    , mPosition(pos)
    , mDimensions(dims)
    , mIsActive(true)
{
}

Button::~Button() = default;

void Button::setClickFunc(const std::function<void()>& onClick) {
    mOnClick = onClick;
}

const Vector2& Button::getPosition() const {
    return mPosition;
}

void Button::setActive(bool value) {
    mIsActive = value;
}

bool Button::getActive() const {
    return mIsActive;
}

bool Button::containsPoint(const Vector2& pt) const {
    bool no = pt.x < (mPosition.x) ||
        pt.x >(mPosition.x + mDimensions.x) ||
        pt.y < (mPosition.y) ||
        pt.y >(mPosition.y + mDimensions.y);
    return !no;
}

bool Button::clickButton(const Vector2& pt) const {
    if (!mIsActive) {
        return false;
    }

    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    if (!containsPoint(pt)) {
        return false;
    }

    onClick();
    return true;
}

void Button::onClick() const {
    if (mOnClick) {
        mOnClick();
    }
}



SpriteButton::SpriteButton(const std::function<void()>& onClick, const std::string& fileName, const Vector2& pos)
    : mOnClick(onClick)
    , mSprite(std::make_shared<Sprite>(fileName))
{
    mSprite->getSpriteManager().add(mSprite);
    mSprite->transform().setPosition(pos);
}

SpriteButton::~SpriteButton() = default;

void SpriteButton::setClickFunc(const std::function<void()>& onClick) {
    mOnClick = onClick;
}

const Vector2& SpriteButton::getPosition() const {
    return mSprite->transform().getPosition();
}

void SpriteButton::setActive(bool value) {
    mSprite->setActive(value);
}

bool SpriteButton::getActive() const {
    return mSprite->getActive();
}

bool SpriteButton::containsPoint(const Vector2& pt) const {
    return SpriteUtility::containsDebug(*mSprite, pt);
}

bool SpriteButton::clickButton(const Vector2& pt) const {
    if (!mSprite->getActive()) {
        return false;
    }

    if (!Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    if (!containsPoint(pt)) {
        return false;
    }

    onClick();
    return true;
}

void SpriteButton::onClick() const {
    if (mOnClick) {
        mOnClick();
    }
}
