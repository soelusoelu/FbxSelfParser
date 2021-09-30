#pragma once

#include "../Math/Math.h"
#include "../System/AssetsDirectoryPath.h"
#include <string>

struct JsonObject;
class Game;
class GameObject;

class LevelLoader {
private:
    LevelLoader() = delete;
    ~LevelLoader() = delete;
    LevelLoader(const LevelLoader&) = delete;
    LevelLoader& operator=(const LevelLoader&) = delete;

public:
    //jsonファイルの読み込み
    static bool loadJson(
        JsonObject& rootObject,
        const std::string& filename,
        const std::string& directoryPath = AssetsDirectoryPath::DATA_PATH
    );
    //jsonファイルの書き込み
    static void saveJson(
        const JsonObject& rootObject,
        const std::string& filename,
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
    //ファイルに書き込む
    static void writeBuffer(
        const JsonObject& inObj,
        const std::string& filePath
    );
};
