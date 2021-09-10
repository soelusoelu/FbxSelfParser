#pragma once

#include "ComponentManager.h"
#include "../GameObject/IThisGetter.h"
#include "../GameObject/Object.h"
#include "../GameObject/GameObject.h"
#include "../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <vector>

class Collider;
class Transform3D;

class Component
    : public Object
{
public:
    Component();
    virtual ~Component();
    //loadPropertiesの直後に呼び出される
    virtual void awake() {};
    //全コンポーネントがロードされたあとに呼ばれる
    virtual void start() {};
    //毎フレーム呼ばれる
    virtual void update() {};
    //全ゲームオブジェクトのアップデート後に呼ばれる
    virtual void lateUpdate() {};
    //終了処理
    virtual void finalize() {};
    //アクティブ・非アクティブ時の切り替え
    virtual void onEnable(bool value) {};
    //衝突した瞬間のコライダーを取得する
    virtual void onCollisionEnter(Collider& other) {};
    //衝突し続けているコライダーを取得する
    virtual void onCollisionStay(Collider& other) {};
    //衝突しなくなった瞬間のコライダーを取得する
    virtual void onCollisionExit(Collider& other) {};
    //ファイルの読み込みと保存
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {};
    //Inspectorに表示する情報
    virtual void drawInspector() {};

    //コンポーネントがアタッチされているゲームオブジェクトを返す
    GameObject& gameObject() const;
    //トランスフォームの取得
    Transform3D& transform() const;
    //コンポーネントの名前を返す
    const std::string& getComponentName() const;

    //コンポーネントの取得
    template<typename T>
    std::shared_ptr<T> getComponent() const {
        return componentManager().getComponent<T>();
    }

    //指定した型のコンポーネントをすべて取得
    template<typename T>
    std::vector<std::shared_ptr<T>> getComponents() const {
        return componentManager().getComponents<T>();
    }

    //コンポーネントの追加
    template <typename T>
    static std::shared_ptr<T> addComponent(GameObject& gameObject, const std::string& componentName) {
        auto t = std::make_shared<T>();
        t->mGameObjectGetter = &gameObject;
        t->mComponentName = componentName;
        t->componentManager().addComponent(t);
        t->awake();
        t->start();
        return t;
    }

    //自身にコンポーネントを追加する
    template <typename T>
    std::shared_ptr<T> addComponent(const std::string& componentName) {
        return addComponent<T>(mGameObjectGetter->getThis(), componentName);
    }

    //指定されたプロパティでコンポーネントを生成
    template <typename T>
    static void create(GameObject& gameObject, const std::string& componentName, rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc) {
        auto t = std::make_shared<T>();
        t->mGameObjectGetter = &gameObject;
        t->mComponentName = componentName;
        t->componentManager().addComponent(t);
        t->saveAndLoad(inObj, alloc, FileMode::LOAD);
        t->awake();
    }

private:
    //自身を管理しているマネージャーを返す
    ComponentManager& componentManager() const;

private:
    IThisGetter<GameObject>* mGameObjectGetter;
    std::string mComponentName;
};
