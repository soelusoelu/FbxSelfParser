#pragma once

#include <memory>

class GameObject;

using GameObjectPtr = std::shared_ptr<GameObject>;

class IGameObjectAdder {
public:
    virtual ~IGameObjectAdder() = default;
    virtual void add(const GameObjectPtr& newGameObject) = 0;
};
