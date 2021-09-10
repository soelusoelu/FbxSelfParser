#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include "../../../Math/Math.h"
#include <functional>
#include <memory>

class SpriteComponent;

class SpriteButtonComponent : public Component {
    using SpritePtr = std::shared_ptr<SpriteComponent>;

public:
    SpriteButtonComponent();
    ~SpriteButtonComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;
    virtual void onEnable(bool value) override;

    //初期化
    void initialize();
    //スプライトを設定する
    void setSprite(const SpritePtr& sprite);
    //ボタン機能の有効化を設定する
    void enableButtonFunction(bool value);
    //クリックした際のコールバック
    void callbackClick(const std::function<void()>& onClick);

private:
    SpriteButtonComponent(const SpriteButtonComponent&) = delete;
    SpriteButtonComponent& operator=(const SpriteButtonComponent&) = delete;

    //2枚のスプライトにアクセスできるか
    bool canAccessSprites() const;
    //スプライトの設定を共通化する
    void spriteSettings(SpriteComponent& dst, const SpriteComponent& src);
    //アクティブにするスプライトの設定
    void activeSpriteSettings(const SpritePtr& active, const SpritePtr& nonActive);
    //スプライトを切り替える
    void swapSprite(bool contains);

private:
    SpritePtr mSprite;
    SpritePtr mSelectingSprite;
    Function<void()> mCallbackClick;
    bool mEnableFunction;
    bool mPreviousContains;
    bool mWaitOneFrame;
};
