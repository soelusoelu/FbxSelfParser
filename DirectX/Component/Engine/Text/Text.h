#pragma once

#include "TextBase.h"
#include <string>

class Text
    : public TextBase
{
public:
    Text();
    ~Text();
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;
    void setText(const std::string& text);
    const std::string& text() const;

private:
    std::string mText;
};
