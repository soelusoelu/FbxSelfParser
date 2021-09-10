#pragma once

#include "../Math/Math.h"
#include "../System/AssetsDirectoryPath.h"
#include <rapidjson/document.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class GameObject;

class GameObjectFactory {
    using ComponentFunc = std::function<void(GameObject&, const std::string&, rapidjson::Value&, rapidjson::Document::AllocatorType&)>;

public:
    GameObjectFactory();
    ~GameObjectFactory();
    //ファイルからゲームオブジェクト生成
    std::shared_ptr<GameObject> createGameObjectFromFile(const std::string& type, const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH);

private:
    GameObjectFactory(const GameObjectFactory&) = delete;
    GameObjectFactory& operator=(const GameObjectFactory&) = delete;

    //ゲームオブジェクトを生成する
    std::shared_ptr<GameObject> createGameObject(rapidjson::Document& inDocument, const std::string& type, const std::string& directoryPath);
    //ゲームオブジェクトのタグを取得する
    std::string loadTag(const rapidjson::Document& inDocument);
    //ゲームオブジェクトプロパティの読み込み
    void loadGameObjectProperties(GameObject& gameObject, rapidjson::Document& inDocument);
    //継承コンポーネントの読み込み
    void loadPrototypeComponents(GameObject& gameObject, const rapidjson::Document& inDocument, const std::string& directoryPath) const;
    //コンポーネントの読み込み
    void loadComponents(GameObject& gameObject, rapidjson::Document& inDocument) const;
    //各コンポーネントの読み込み
    void loadComponent(GameObject& gameObject, rapidjson::Value& component, rapidjson::Document::AllocatorType& alloc) const;

    //有効な型か
    bool isValidType(std::string& outType, const rapidjson::Value& inObj) const;

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
