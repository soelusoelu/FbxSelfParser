#include "LevelLoader.h"
#include "FileMode.h"
#include "FileUtil.h"
#include "JsonHelper.h"
#include "../Component/ComponentManager.h"
#include "../Engine/DebugManager/DebugUtility/Debug.h"
#include "../GameObject/GameObject.h"
#include "../System/Game.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <fstream>

bool LevelLoader::loadJSON(
    rapidjson::Document& outDoc,
    const std::string& fileName,
    const std::string& directoryPath
) {
    const auto& filePath = directoryPath + fileName;

    //バイナリモードで開き、末尾に移動
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Debug::windowMessage(filePath + "ファイルが見つかりません");
        return false;
    }

    //ファイルの末尾 = ファイルサイズ
    std::ifstream::pos_type fileSize = file.tellg();
    //ファイルの先頭まで戻る
    file.seekg(0, std::ios::beg);

    //末尾のヌルを含めたsize+1の配列を作る
    std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
    //配列にバイト列を書き込む
    file.read(bytes.data(), static_cast<size_t>(fileSize));

    //生データをRapidJSONドキュメントにロードする
    outDoc.Parse(bytes.data());
    if (!outDoc.IsObject()) {
        Debug::windowMessage(filePath + "ファイルは有効ではありません");
        return false;
    }

    return true;
}

void LevelLoader::loadGlobal(
    Game* root,
    const std::string& filePath
) {
    rapidjson::Document doc;
    if (!loadJSON(doc, filePath)) {
        return;
    }

    rapidjson::Value& globals = doc["globalProperties"];
    if (!globals.IsObject()) {
        Debug::windowMessage(filePath + ": [globalProperties]が見つからないか、正しいオブジェクトではありません");
    }

    root->saveAndLoad(globals, doc.GetAllocator(), FileMode::LOAD);
}

void LevelLoader::saveGlobal(
    Game* root,
    const std::string& fileName,
    const std::string& directoryPath
) {
    //ドキュメントとルートオブジェクトを生成
    rapidjson::Document doc;
    doc.SetObject();

    //アロケータの取得
    rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();

    rapidjson::Value props(rapidjson::kObjectType);
    root->saveAndLoad(props, alloc, FileMode::SAVE);
    doc.AddMember("globalProperties", props, alloc);

    //文字列をファイルに書き込む
    writeBuffer(doc, directoryPath + fileName);
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
    //ドキュメントとルートオブジェクトを生成
    rapidjson::Document doc;
    doc.SetObject();

    //アロケータの取得
    rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();

    //タグを追加
    JsonHelper::setString(gameObject.tag(), "tag", doc, alloc);

    //トランスフォーム用のjsonオブジェクトを作る
    rapidjson::Value props(rapidjson::kObjectType);
    //トランスフォームを保存
    gameObject.saveAndLoad(props, alloc, FileMode::SAVE);
    //トランスフォームをゲームオブジェクトのjsonオブジェクトに追加
    doc.AddMember("transform", props, alloc);

    //コンポーネントを保存
    rapidjson::Value components(rapidjson::kArrayType);
    gameObject.componentManager().saveComponents(alloc, components);
    doc.AddMember("components", components, alloc);

    //文字列をファイルに書き込む
    writeBuffer(doc, directoryPath + filename);
}

void LevelLoader::saveOnlyComponents(
    const GameObject& gameObject,
    const std::string& filename,
    const std::string& directoryPath
) {
    //ドキュメントとルートオブジェクトを生成
    rapidjson::Document doc;
    doc.SetObject();

    //アロケータの取得
    rapidjson::Document::AllocatorType& alloc = doc.GetAllocator();

    //コンポーネントを保存
    rapidjson::Value components(rapidjson::kArrayType);
    gameObject.componentManager().saveComponents(alloc, components);
    doc.AddMember("components", components, alloc);

    //文字列をファイルに書き込む
    writeBuffer(doc, directoryPath + filename);
}

void LevelLoader::writeBuffer(
    const rapidjson::Document& inDoc,
    const std::string& filePath
) {
    //jsonを文字列バッファに保存
    rapidjson::StringBuffer buffer;
    //整形出力用にPrettyWriterを使う(もしくはWriter)
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    inDoc.Accept(writer);
    const char* output = buffer.GetString();

    //文字列をファイルに書き込む
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << output;
    }
}
