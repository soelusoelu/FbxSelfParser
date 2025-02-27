﻿#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>
#include <string>

class Shader;
class Sprite;
class SpriteManager;
class Texture;
class Transform2D;

class SpriteComponent
    : public Component
    , public std::enable_shared_from_this<SpriteComponent>
{
public:
    SpriteComponent();
    virtual ~SpriteComponent();
    virtual void awake() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;
    virtual void drawInspector() override;

    //描画
    void draw(const Matrix4& proj) const;

    //トランスフォーム
    Transform2D& transform() const;
    //色味 [0, 1]
    void setColor(const Vector3& color);
    void setColor(float r, float g, float b);
    //色を取得する
    const Vector3& getColor() const;
    //不透明度 [0, 1]
    void setAlpha(float alpha);
    //不透明度を取得する
    float getAlpha() const;
    //切り取り範囲(left, top, right, bottom, 0～1)
    void setUV(float l, float t, float r, float b);
    const Vector4& getUV() const;
    //テクスチャの横幅を取得
    unsigned getWidth() const;
    //テクスチャの縦幅を取得
    unsigned getHeight() const;
    //状態管理
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;
    //ファイル名からテクスチャを設定する
    void setTextureFromFileName(const std::string& fileName);
    //テクスチャを設定する
    void setTexture(const std::shared_ptr<Texture>& texture);
    //テクスチャ
    const Texture& texture() const;
    //テクスチャIDを取得する
    int getTextureID() const;
    //シェーダーの取得
    const Shader& shader() const;
    //ファイル名の取得
    const std::string& fileName() const;
    //描画優先番号の設定
    //重いので注意
    void setDrawOrder(int order);
    //描画優先番号の取得
    int getDrawOrder() const;

    //ソート用
    static bool compare(const std::shared_ptr<SpriteComponent>& lhs, const std::shared_ptr<SpriteComponent>& rhs);

    static void setSpriteManager(SpriteManager* manager);

private:
    void addToManager();

protected:
    std::unique_ptr<Sprite> mSprite;
    int mDrawOrder;

    static inline SpriteManager* mSpriteManager = nullptr;
};

