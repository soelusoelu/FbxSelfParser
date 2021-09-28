#pragma once

#include "TextBase.h"

class TextFloat
    : public TextBase
{
public:
    TextFloat();
    ~TextFloat();
    virtual void lateUpdate() override;
    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;
    virtual void drawInspector() override;
    void setNumber(float number);
    float number() const;
    void setDecimalDigits(int decimalDigits);
    int getDecimalDigits() const;

private:
    float mNumber;
    int mDecimalDigits;
};
