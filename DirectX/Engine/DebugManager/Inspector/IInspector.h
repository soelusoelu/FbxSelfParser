#pragma once

#include <memory>

class GameObject;

class IInspector {
public:
    virtual ~IInspector() = default;
    //インスペクターに表示するゲームオブジェクトを設定する
    virtual void setTarget(const std::shared_ptr<GameObject>& target) = 0;
    //インスペクターの表示位置Xを取得する
    virtual float getInspectorPositionX() const = 0;
};
