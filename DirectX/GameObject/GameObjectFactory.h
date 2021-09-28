#pragma once

#include "../Math/Math.h"
#include "../System/AssetsDirectoryPath.h"
#include "../System/Json/JsonObject.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class GameObject;

class GameObjectFactory {
    using ComponentFunc = std::function<void(GameObject&, const std::string&, JsonObject&)>;

public:
    GameObjectFactory();
    ~GameObjectFactory();
    GameObjectFactory(const GameObjectFactory&) = delete;
    GameObjectFactory& operator=(const GameObjectFactory&) = delete;

    //ファイルからゲームオブジェクト生成
    std::shared_ptr<GameObject> createGameObjectFromFile(const std::string& type, const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH);

private:
    //ゲームオブジェクトを生成する
    std::shared_ptr<GameObject> createGameObject(JsonObject& inObj, const std::string& type, const std::string& directoryPath);
    //ゲームオブジェクトのタグを取得する
    std::string loadTag(const JsonObject& inObj);
    //ゲームオブジェクトプロパティの読み込み
    void loadGameObjectProperties(GameObject& gameObject, JsonObject& inObj);
    //継承コンポーネントの読み込み
    void loadPrototypeComponents(GameObject& gameObject, const JsonObject& inObj, const std::string& directoryPath) const;
    //コンポーネントの読み込み
    void loadComponents(GameObject& gameObject, JsonObject& inObj) const;
    //各コンポーネントの読み込み
    void loadComponent(GameObject& gameObject, JsonObject& component) const;

    //有効な型か
    bool isValidType(std::string& outType, const JsonObject& inObj) const;

private:
    std::unordered_map<std::string, ComponentFunc> mComponents;

    static inline bool mInstantiated = false;
};

class GameObjectCreater {
public:
    static void initialize();
    static void finalize();
    static std::shared_ptr<GameObject> create(const std::string& type, const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH);

private:
    GameObjectCreater() = delete;
    ~GameObjectCreater() = delete;
    GameObjectCreater(const GameObjectCreater&) = delete;
    GameObjectCreater& operator=(const GameObjectCreater&) = delete;

private:
    static inline GameObjectFactory* mFactory = nullptr;
};
