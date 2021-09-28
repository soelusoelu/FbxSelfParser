#include "ComponentManager.h"
#include "Component.h"
#include "../Utility/JsonHelper.h"

ComponentManager::ComponentManager() = default;
ComponentManager::~ComponentManager() = default;

void ComponentManager::start() {
    //addComponentで追加されたものは無視(addComponent内で処理するため)
    const auto COMPONENT_COUNT = mComponents.size();
    for (size_t i = 0; i < COMPONENT_COUNT; ++i) {
        mComponents[i]->start();
    }
}

void ComponentManager::update() const {
    for (const auto& comp : mComponents) {
        if (comp->isDead()) {
            continue;
        }

        comp->update();
        comp->updateDestroyTimer();
    }
}

void ComponentManager::lateUpdate() const {
    for (const auto& comp : mComponents) {
        if (comp->isDead()) {
            continue;
        }

        comp->lateUpdate();
    }
}

void ComponentManager::destroy() {
    auto itr = mComponents.begin();
    while (itr != mComponents.end()) {
        if ((*itr)->isDead()) {
            (*itr)->finalize();
            itr = mComponents.erase(itr);
        } else {
            ++itr;
        }
    }
}

void ComponentManager::finalize() {
    for (const auto& comp : mComponents) {
        comp->finalize();
    }

    mComponents.clear();
}

void ComponentManager::addComponent(const ComponentPtr& component) {
    mComponents.emplace_back(component);
}

void ComponentManager::onEnable(bool value) const {
    for (const auto& comp : mComponents) {
        comp->onEnable(value);
    }
}

void ComponentManager::onCollisionEnter(Collider& other) const {
    for (const auto& comp : mComponents) {
        comp->onCollisionEnter(other);
    }
}

void ComponentManager::onCollisionStay(Collider& other) const {
    for (const auto& comp : mComponents) {
        comp->onCollisionStay(other);
    }
}

void ComponentManager::onCollisionExit(Collider& other) const {
    for (const auto& comp : mComponents) {
        comp->onCollisionExit(other);
    }
}

const std::vector<std::shared_ptr<Component>>& ComponentManager::getAllComponents() const {
    return mComponents;
}

void ComponentManager::saveComponents(JsonObjectArray& inObj) const {
    for (const auto& c : mComponents) {
        saveComponent(*inObj.emplace_back(), *c);
    }
}

void ComponentManager::saveComponent(JsonObject& outArray, Component& component) const {
    //コンポーネント名を保存
    JsonHelper::setString(component.getComponentName(), "type", outArray);

    //プロパティ用オブジェクトを作成
    auto& props = outArray.children.emplace("properties", std::make_shared<JsonObject>()).first->second;
    //コンポーネントのプロパティを保存する
    component.saveAndLoad(*props, FileMode::SAVE);
}
