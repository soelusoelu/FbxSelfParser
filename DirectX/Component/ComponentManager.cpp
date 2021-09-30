#include "ComponentManager.h"
#include "Component.h"
#include "../System/Json/JsonObject.h"
#include "../System/Json/JsonValue.h"
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

void ComponentManager::saveComponents(JsonValue& out) const {
    if (!out.isArray()) {
        out.setArray();
    }
    auto& a = out.a;
    auto size = mComponents.size();
    a.resize(size);
    for (size_t i = 0; i < size; ++i) {
        a[i].setObject();
        saveComponent(a[i].getObject(), *mComponents[i]);
    }
}

void ComponentManager::saveComponent(JsonObject& out, Component& component) const {
    //コンポーネント名を保存
    JsonHelper::setString(component.getComponentName(), "type", out);

    //プロパティ用オブジェクトを作成
    auto props = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
    //コンポーネントのプロパティを保存する
    component.saveAndLoad(props->getObject(), FileMode::SAVE);
    out.setValue("properties", props);
}
