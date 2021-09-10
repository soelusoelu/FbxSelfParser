#pragma once

#include "TextBase.h"

class TextNumber
    : public TextBase
{
public:
    TextNumber();
    ~TextNumber();
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;
    void setNumber(int number);
    int number() const;

private:
    int mNumber;
};
