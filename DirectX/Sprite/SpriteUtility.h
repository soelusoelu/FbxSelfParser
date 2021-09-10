#pragma once

#include "../Math/Math.h"

class Sprite;
class SpriteComponent;
class Transform2D;

//スプライト補助クラス
class SpriteUtility {
public:
    //スプライトの中に指定の座標が含まれているか
    static bool contains(const Sprite& sprite, const Vector2& position);
    //スプライトコンポーネントの中に指定の座標が含まれているか
    static bool contains(const SpriteComponent& sprite, const Vector2& position);
    //デバッグ画面のスプライトの中に指定の座標が含まれているか
    static bool containsDebug(const Sprite& sprite, const Vector2& position);
    //スプライトをマウスの左ボタンでクリックしたか
    static bool clickSprite(const Sprite& sprite);
    //スプライトコンポーネントをマウスの左ボタンでクリックしたか
    static bool clickSprite(const SpriteComponent& sprite);
    //デバッグ画面のスプライトをマウスの左ボタンでクリックしたか
    static bool clickSpriteDebug(const Sprite& sprite);

private:
    //トランスフォームと画像サイズから指定の座標が含まれているか調べる
    static bool containsTexture(const Transform2D& transform, const Vector2& textureSize, const Vector2& targetPosition);
    //トランスフォームと画像サイズから指定の座標が含まれているか調べる
    static bool containsTextureDebug(const Transform2D& transform, const Vector2& textureSize, const Vector2& targetPosition);

private:
    SpriteUtility() = delete;
    ~SpriteUtility() = delete;
};
