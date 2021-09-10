#pragma once

#include "../Math/Math.h"
#include "../System/AssetsDirectoryPath.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Game;
class GameObject;

class LevelLoader {
private:
    LevelLoader() = delete;
    ~LevelLoader() = delete;

public:
    //jsonファイルの読み込み
    static bool loadJSON(
        rapidjson::Document& outDoc,
        const std::string& fileName,
        const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH
    );
    //グローバルデータを読み込む
    static void loadGlobal(
        Game* root,
        const std::string& filePath
    );
    //グローバルデータを書き込む
    static void saveGlobal(
        Game* root,
        const std::string& fileName,
        const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH
    );
    //ゲームオブジェクトを保存する
    static void saveGameObject(
        GameObject& gameObject,
        const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH
    );
    //ゲームオブジェクトをファイル名を指定して保存する
    static void saveGameObject(
        GameObject& gameObject,
        const std::string& filename,
        const std::string& directoryPath
    );
    //コンポーネントのみ保存する
    static void saveOnlyComponents(
        const GameObject& gameObject,
        const std::string& filename,
        const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH
    );

private:
    LevelLoader(const LevelLoader&) = delete;
    LevelLoader& operator=(const LevelLoader&) = delete;

    //ファイルに書き込む
    static void writeBuffer(
        const rapidjson::Document& inDoc,
        const std::string& filePath
    );
};
