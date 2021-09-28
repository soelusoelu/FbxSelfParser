#include "LevelLoader.h"
#include "FileMode.h"
#include "FileUtil.h"
#include "JsonHelper.h"
#include "../Component/ComponentManager.h"
#include "../Engine/DebugManager/DebugUtility/Debug.h"
#include "../GameObject/GameObject.h"
#include "../System/Game.h"
#include "../System/Json/JsonObject.h"
#include "../System/Json/JsonReader.h"
#include "../System/Json/JsonStream.h"
#include <memory>

bool LevelLoader::loadJson(
    JsonObject& rootObject,
    const std::string& filename,
    const std::string& directoryPath
) {
    const auto& filePath = directoryPath + filename;

    auto stream = std::make_unique<JsonStream>(filePath.c_str());
    auto reader = std::make_unique<JsonReader>();
    reader->parse(*stream, rootObject);

    return true;
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
    auto props = std::make_shared<JsonObject>();
    //トランスフォームを保存
    gameObject.saveAndLoad(*props, FileMode::SAVE);
    rootObject.children.emplace("transform", props);

    //コンポーネントを保存
    JsonObjectArray components;
    gameObject.componentManager().saveComponents(components);
    rootObject.arrayChildren.emplace("components", components);

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
    JsonObjectArray components;
    gameObject.componentManager().saveComponents(components);
    rootObject.arrayChildren.emplace("components", components);

    //文字列をファイルに書き込む
    writeBuffer(rootObject, directoryPath + filename);
}

void LevelLoader::writeBuffer(
    const JsonObject& inObj,
    const std::string& filePath
) {
    ////jsonを文字列バッファに保存
    //rapidjson::StringBuffer buffer;
    ////整形出力用にPrettyWriterを使う(もしくはWriter)
    //rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    //inDoc.Accept(writer);
    //const char* output = buffer.GetString();

    ////文字列をファイルに書き込む
    //std::ofstream outFile(filePath);
    //if (outFile.is_open()) {
    //    outFile << output;
    //}
}
