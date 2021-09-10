#pragma once

#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <vector>

class Component;
class Collider;

class ComponentManager {
    using ComponentPtr = std::shared_ptr<Component>;
    using ComponentPtrArray = std::vector<ComponentPtr>;

public:
    ComponentManager();
    ~ComponentManager();
    //各コンポーネントのstartを一度だけ実行
    void start();
    //所有するすべてのコンポーネントを更新
    void update() const;
    //所有するすべてのコンポーネントを遅延更新
    void lateUpdate() const;
    //所有するすべてのコンポーネントから削除命令が出ているものを削除
    void destroy();
    //所有するすべてのコンポーネントの終了処理を実行
    void finalize();
    //コンポーネントの追加
    void addComponent(const ComponentPtr& component);

    //所有するすべてのコンポーネントのonSetActiveを実行
    void onEnable(bool value) const;
    //所有するすべてのコンポーネントのonCollisionEnterを実行
    void onCollisionEnter(Collider& other) const;
    //所有するすべてのコンポーネントのonCollisionStayを実行
    void onCollisionStay(Collider& other) const;
    //所有するすべてのコンポーネントのonCollisionExitを実行
    void onCollisionExit(Collider& other) const;

    //全コンポーネントの取得
    const ComponentPtrArray& getAllComponents() const;

    //コンポーネントの取得
    template<typename T>
    std::shared_ptr<T> getComponent() const {
        std::shared_ptr<T> comp = nullptr;
        for (const auto& c : mComponents) {
            comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                return comp;
            }
        }
        //最後まで見つからなければnullptrを返す
        return comp;
    }

    //指定した型のコンポーネントをすべて取得
    template<typename T>
    std::vector<std::shared_ptr<T>> getComponents() const {
        std::vector<std::shared_ptr<T>> components;
        for (const auto& c : mComponents) {
            auto comp = std::dynamic_pointer_cast<T>(c);
            if (comp) {
                components.emplace_back(comp);
            }
        }
        return components;
    }

    //すべてのコンポーネントを保存する
    void saveComponents(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

private:
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;

    //各コンポーネントを保存する
    void saveComponent(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& outArray, Component& component) const;

private:
    ComponentPtrArray mComponents;
};
