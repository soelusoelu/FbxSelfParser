#pragma once

#include <list>
#include <memory>

class GameObject;

using GameObjectPtr = std::shared_ptr<GameObject>;
using GameObjectPtrList = std::list<GameObjectPtr>;

class IGameObjectsGetter {
public:
    virtual ~IGameObjectsGetter() = default;
    virtual const GameObjectPtrList& getGameObjects() const = 0;
};
