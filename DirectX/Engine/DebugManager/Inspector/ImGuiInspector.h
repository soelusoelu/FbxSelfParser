#pragma once

#include "IInspector.h"
#include "../../../Device/FileOperator.h"
#include <memory>

class GameObject;

//ImGuiを使用したインスペクター
class ImGuiInspector
    : public FileOperator
    , public IInspector
{
public:
    ImGuiInspector();
    ~ImGuiInspector();
    virtual void setTarget(const std::shared_ptr<GameObject>& target) override;
    virtual float getInspectorPositionX() const override;

    void drawInspect() const;

private:
    ImGuiInspector(const ImGuiInspector&) = delete;
    ImGuiInspector& operator=(const ImGuiInspector&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    void drawName(const GameObject& target) const;
    void drawTag(const GameObject& target) const;

private:
    std::weak_ptr<GameObject> mTarget;
    float mInspectorPositionX;
};
