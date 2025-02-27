﻿#include "SpriteUtility.h"
#include "../Collision/Collision.h"
#include "../Component/Engine/Sprite/SpriteComponent.h"
#include "../Input/Input.h"
#include "../Sprite/Sprite.h"
#include "../System/Window.h"
#include "../Transform/Transform2D.h"

bool SpriteUtility::contains(const Sprite& sprite, const Vector2& position) {
    return containsTexture(sprite.transform(), Vector2(sprite.getWidth(), sprite.getHeight()), position);
}

bool SpriteUtility::contains(const SpriteComponent& sprite, const Vector2& position) {
    return containsTexture(sprite.transform(), Vector2(sprite.getWidth(), sprite.getHeight()), position);
}

bool SpriteUtility::containsDebug(const Sprite& sprite, const Vector2& position) {
    return containsTextureDebug(sprite.transform(), Vector2(sprite.getWidth(), sprite.getHeight()), position);
}

bool SpriteUtility::clickSprite(const Sprite& sprite) {
    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押してないなら終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    return contains(sprite, mouse.getMousePosition());
}

bool SpriteUtility::clickSprite(const SpriteComponent& sprite) {
    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押してないなら終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    return contains(sprite, mouse.getMousePosition());
}

bool SpriteUtility::clickSpriteDebug(const Sprite& sprite) {
    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押してないなら終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    return containsDebug(sprite, mouse.getMousePosition());
}

bool SpriteUtility::containsTexture(const Transform2D& transform, const Vector2& textureSize, const Vector2& targetPosition) {
    //ウィンドウ補正値を取得する
    auto compen = Window::getWindowCorrect();

    //計算に必要な要素を取得する
    const auto& scale = transform.getScale();
    const auto& pivot = transform.getPivot() * scale;
    const auto& compenPos = (transform.getPosition() - pivot) * compen;
    const auto& compenScale = scale * compen;
    const auto& texSize = textureSize * compenScale;

    //スプライトをもとに矩形作成
    Square square(compenPos, compenPos + texSize);

    //矩形の中にマウスの座標が含まれているか
    return square.contains(targetPosition);
}

bool SpriteUtility::containsTextureDebug(const Transform2D& transform, const Vector2& textureSize, const Vector2& targetPosition) {
    //計算に必要な要素を取得する
    const auto& scale = transform.getScale();
    const auto& pivot = transform.getPivot() * scale;
    const auto& compenPos = transform.getPosition() - pivot;
    const auto& texSize = textureSize * scale;

    //スプライトをもとに矩形作成
    Square square(compenPos, compenPos + texSize);

    //矩形の中にマウスの座標が含まれているか
    return square.contains(targetPosition);
}
