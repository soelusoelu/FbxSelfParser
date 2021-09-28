#pragma once

#include "../../Component.h"
#include <string>
#include <vector>

//ゲームオブジェクトのセーブ/ロードを請け負うクラス
class GameObjectSaveAndLoader : public Component {
    using StringArray = std::vector<std::string>;

public:
    GameObjectSaveAndLoader();
    ~GameObjectSaveAndLoader();
    GameObjectSaveAndLoader(const GameObjectSaveAndLoader&) = delete;
    GameObjectSaveAndLoader& operator=(const GameObjectSaveAndLoader&) = delete;

    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;

    //保存するゲームオブジェクトを追加する
    void addSaveGameObject(const std::string& name);

private:
    //保存するゲームオブジェクト名配列
    StringArray mGameObjectNames;
};
