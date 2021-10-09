#include "LevelLoader.h"
#include "FileMode.h"
#include "FileUtil.h"
#include "JsonHelper.h"
#include "../Component/ComponentManager.h"
#include "../Engine/DebugManager/DebugUtility/Debug.h"
#include "../GameObject/GameObject.h"
#include "../System/Game.h"
#include "../System/Json/JsonInputStream.h"
#include "../System/Json/JsonObject.h"
#include "../System/Json/JsonReader.h"
#include "../System/Json/JsonValue.h"
#include "../System/Json/JsonWriter.h"
#include <memory>

bool LevelLoader::loadJson(
    JsonObject& rootObject,
    const std::string& filename,
    const std::string& directoryPath
) {
    const auto& filePath = directoryPath + filename;

    auto stream = std::make_unique<JsonInputStream>(filePath.c_str());
    auto reader = std::make_unique<JsonReader>();
    reader->parse(*stream, rootObject);

    return true;
}

void LevelLoader::saveJson(
    const JsonObject& rootObject,
    const std::string& filename,
    const std::string& directoryPath
) {
    writeBuffer(rootObject, directoryPath + filename);
}

void LevelLoader::saveGameObject(
    GameObject& gameObject,
    const std::string& directoryPath
) {
    saveGameObject(gameObject, gameObject.name() + ".json", directoryPath);
}

void LevelLoader::saveGameObject(
    GameObject& gameObject,
    const std::string& filename,
    const std::string& directoryPath
) {
    //ルートオブジェクトを生成
    JsonObject rootObject;

    //タグを追加
    JsonHelper::setString(gameObject.tag(), "tag", rootObject);

    //トランスフォーム用のjsonオブジェクトを作る
    auto props = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
    //トランスフォームを保存
    gameObject.saveAndLoad(props->getObject(), FileMode::SAVE);
    rootObject.setValue("transform", props);

    //コンポーネントを保存
    auto components = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    gameObject.componentManager().saveComponents(*components);
    rootObject.setValue("components", components);

    //文字列をファイルに書き込む
    writeBuffer(rootObject, directoryPath + filename);
}

void LevelLoader::saveOnlyComponents(
    const GameObject& gameObject,
    const std::string& filename,
    const std::string& directoryPath
) {
    //ルートオブジェクトを生成
    JsonObject rootObject;

    //コンポーネントを保存
    auto components = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    gameObject.componentManager().saveComponents(*components);
    rootObject.setValue("components", components);

    //文字列をファイルに書き込む
    writeBuffer(rootObject, directoryPath + filename);
}

void LevelLoader::writeBuffer(
    const JsonObject& inObj,
    const std::string& filePath
) {
    auto writer = std::make_unique<JsonWriter>();
    writer->write(filePath.c_str(), inObj);
}
