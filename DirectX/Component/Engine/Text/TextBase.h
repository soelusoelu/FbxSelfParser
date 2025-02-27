﻿#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Transform/Pivot.h"
#include <string>

class DrawString;

class TextBase
    : public Component
{
public:
    TextBase();
    virtual ~TextBase();
    virtual void onEnable(bool value) override;
    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;
    virtual void drawInspector() override;
    void setPosition(const Vector2& pos);
    const Vector2& getPosition() const;
    void setScale(const Vector2& scale);
    const Vector2& getScale() const;
    void setColor(const Vector3& color);
    const Vector3& getColor() const;
    void setAlpha(float alpha);
    float getAlpha() const;
    void setPivot(Pivot pivot);
    Pivot getPivot() const;
    void setActive(bool value);
    bool getActive() const;

    static void setDrawString(DrawString* drawString);

protected:
    Vector2 mPosition;
    Vector2 mScale;
    Vector3 mColor;
    float mAlpha;
    Pivot mPivot;
    bool mIsActive;

    static inline DrawString* mDrawString = nullptr;
};
