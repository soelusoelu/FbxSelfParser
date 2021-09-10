#include "GameObjectFactory.h"
#include "GameObject.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Camera/CameraMove.h"
#include "../Component/Engine/CollideOperation/AABBMouseScaler.h"
#include "../Component/Engine/CollideOperation/AABBSelector.h"
#include "../Component/Engine/CollideOperation/CollideAdder.h"
#include "../Component/Engine/CollideOperation/CollideMouseOperator.h"
#include "../Component/Engine/CollideOperation/MeshAdder.h"
#include "../Component/Engine/Collider/AABBCollider.h"
#include "../Component/Engine/Collider/AABBAnimationCollider.h"
#include "../Component/Engine/Collider/CircleCollider.h"
#include "../Component/Engine/Collider/OBBAnimationCollider.h"
#include "../Component/Engine/Collider/OBBCollider.h"
#include "../Component/Engine/Collider/SphereCollider.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Component/Engine/Light/PointLightComponent.h"
#include "../Component/Engine/Mesh/AnimationCPU.h"
#include "../Component/Engine/Mesh/MeshComponent.h"
#include "../Component/Engine/Mesh/MeshOutLine.h"
#include "../Component/Engine/Mesh/MeshRenderer.h"
#include "../Component/Engine/Mesh/MeshShader.h"
#include "../Component/Engine/Mesh/SkinMeshComponent.h"
#include "../Component/Engine/Other/GameObjectSaveAndLoader.h"
#include "../Component/Engine/Other/SaveThis.h"
#include "../Component/Engine/Other/SimpleMotionChanger.h"
#include "../Component/Engine/Sample/RayMouse.h"
#include "../Component/Engine/Sample/WaveformRenderSample.h"
#include "../Component/Engine/Scene/MapEditor.h"
#include "../Component/Engine/Sound/ListenerComponent.h"
#include "../Component/Engine/Sound/SoundComponent.h"
#include "../Component/Engine/Sprite/MeshRenderOnTextureComponent.h"
#include "../Component/Engine/Sprite/Sprite3D.h"
#include "../Component/Engine/Sprite/SpriteButtonComponent.h"
#include "../Component/Engine/Sprite/SpriteComponent.h"
#include "../Component/Engine/Text/Text.h"
#include "../Component/Engine/Text/TextFloat.h"
#include "../Component/Engine/Text/TextNumber.h"
#include "../Engine/DebugManager/DebugUtility/Debug.h"
#include "../System/GlobalFunction.h"
#include "../Utility/JsonHelper.h"
#include "../Utility/LevelLoader.h"
#include <cassert>

//練習がてら、ちょい楽できるように
#define ADD_COMPONENT(className) { mComponents.emplace((#className), &Component::create<className>); }

GameObjectFactory::GameObjectFactory() {
    assert(!mInstantiated);
    mInstantiated = true;

#pragma region Engine
    ADD_COMPONENT(Camera);
    ADD_COMPONENT(CameraMove);

    ADD_COMPONENT(AABBMouseScaler);
    ADD_COMPONENT(AABBSelector);
    ADD_COMPONENT(CollideAdder);
    ADD_COMPONENT(CollideMouseOperator);
    ADD_COMPONENT(MeshAdder);

    ADD_COMPONENT(AABBCollider);
    ADD_COMPONENT(AABBAnimationCollider);
    ADD_COMPONENT(CircleCollider);
    ADD_COMPONENT(OBBAnimationCollider);
    ADD_COMPONENT(OBBCollider);
    ADD_COMPONENT(SphereCollider);

    ADD_COMPONENT(DirectionalLight);
    ADD_COMPONENT(PointLightComponent);

    ADD_COMPONENT(AnimationCPU);
    ADD_COMPONENT(MeshComponent);
    ADD_COMPONENT(MeshOutLine);
    ADD_COMPONENT(MeshRenderer);
    ADD_COMPONENT(MeshShader);
    ADD_COMPONENT(SkinMeshComponent);

    ADD_COMPONENT(GameObjectSaveAndLoader);
    ADD_COMPONENT(SaveThis);
    ADD_COMPONENT(SimpleMotionChanger);

    ADD_COMPONENT(RayMouse);
    ADD_COMPONENT(WaveformRenderSample);

    ADD_COMPONENT(MapEditor);

    ADD_COMPONENT(ListenerComponent);
    ADD_COMPONENT(SoundComponent);

    ADD_COMPONENT(MeshRenderOnTextureComponent);
    ADD_COMPONENT(Sprite3D);
    ADD_COMPONENT(SpriteButtonComponent);
    ADD_COMPONENT(SpriteComponent);

    ADD_COMPONENT(Text);
    ADD_COMPONENT(TextFloat);
    ADD_COMPONENT(TextNumber);
#pragma endregion
}

GameObjectFactory::~GameObjectFactory() {
    mInstantiated = false;
}

std::shared_ptr<GameObject> GameObjectFactory::createGameObjectFromFile(const std::string& type, const std::string& directoryPath) {
    rapidjson::Document document;
    const auto& fileName = type + ".json";
    if (!LevelLoader::loadJSON(document, fileName, directoryPath)) {
        Debug::windowMessage(directoryPath + fileName + ": ファイルのロードに失敗しました");
        return nullptr;
    }

    return createGameObject(document, type, directoryPath);
}

std::shared_ptr<GameObject> GameObjectFactory::createGameObject(rapidjson::Document& inDocument, const std::string& type, const std::string& directoryPath) {
    //タグを読み込む
    const auto& tag = loadTag(inDocument);
    //ゲームオブジェクトを生成
    auto gameObject = GameObject::create(type, tag);
    //プロパティを読み込む
    loadGameObjectProperties(*gameObject, inDocument);

    //継承コンポーネントがあれば取得
    loadPrototypeComponents(*gameObject, inDocument, directoryPath);
    //コンポーネントがあれば取得
    loadComponents(*gameObject, inDocument);

    //全コンポーネントのstartを呼び出す
    gameObject->componentManager().start();

    return gameObject;
}

std::string GameObjectFactory::loadTag(const rapidjson::Document& inDocument) {
    //初期タグをNoneにする
    std::string tag = "None";
    //タグ属性があれば読み込む
    JsonHelper::getString(tag, "tag", inDocument);

    return tag;
}

void GameObjectFactory::loadGameObjectProperties(GameObject& gameObject,  rapidjson::Document& inDocument) {
    if (inDocument.HasMember("transform")) {
        gameObject.saveAndLoad(inDocument["transform"], inDocument.GetAllocator(), FileMode::LOAD);
    }
}

void GameObjectFactory::loadPrototypeComponents(GameObject& gameObject, const rapidjson::Document& inDocument, const std::string& directoryPath) const {
    //ファイルにprototypeメンバがなければ終了
    if (!inDocument.HasMember("prototype")) {
        return;
    }

    //継承コンポーネントのファイル名を取得する
    std::string prototype;
    JsonHelper::getString(prototype, "prototype", inDocument);

    rapidjson::Document document;
    const auto& fileName = prototype + ".json";
    if (!LevelLoader::loadJSON(document, fileName, directoryPath)) {
        Debug::windowMessage(directoryPath + fileName + ": ファイルのロードに失敗しました");
        return;
    }

    //継承コンポーネント読み込み
    loadComponents(gameObject, document);
}

void GameObjectFactory::loadComponents(GameObject& gameObject, rapidjson::Document& inDocument) const {
    //ファイルにcomponentsメンバがなければ終了
    if (!inDocument.HasMember("components")) {
        return;
    }

    auto& components = inDocument["components"];
    //componentsメンバが配列じゃなければなければ終了
    if (!components.IsArray()) {
        return;
    }

    rapidjson::Document::AllocatorType& alloc = inDocument.GetAllocator();
    for (rapidjson::SizeType i = 0; i < components.Size(); ++i) {
        //各コンポーネントを読み込んでいく
        loadComponent(gameObject, components[i], alloc);
    }
}

void GameObjectFactory::loadComponent(GameObject& gameObject, rapidjson::Value& component, rapidjson::Document::AllocatorType& alloc) const {
    //有効なオブジェクトか
    if (!component.IsObject()) {
        return;
    }
    //有効な型名か
    std::string type;
    if (!isValidType(type, component)) {
        return;
    }
    //mapに存在するか
    auto itr = mComponents.find(type);
    if (itr == mComponents.end()) {
        Debug::windowMessage(type + "は有効な型ではありません");
        return;
    }
    //プロパティがあるか
    if (!component.HasMember("properties")) {
        return;
    }

    //新規コンポーネントを生成
    itr->second(gameObject, type, component["properties"], alloc);
}

bool GameObjectFactory::isValidType(std::string& outType, const rapidjson::Value& inObj) const {
    return (JsonHelper::getString(outType, "type", inObj));
}



void GameObjectCreater::initialize() {
    mFactory = new GameObjectFactory();
}

void GameObjectCreater::finalize() {
    safeDelete(mFactory);
}

std::shared_ptr<GameObject> GameObjectCreater::create(const std::string& type, const std::string& directoryPath) {
    return mFactory->createGameObjectFromFile(type, directoryPath);
}
