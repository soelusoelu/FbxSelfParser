#include "GameObjectSaveAndLoader.h"
#include "../../../GameObject/GameObjectFactory.h"
#include "../../../Utility/JsonHelper.h"

GameObjectSaveAndLoader::GameObjectSaveAndLoader()
    : Component()
{
}

GameObjectSaveAndLoader::~GameObjectSaveAndLoader() = default;

void GameObjectSaveAndLoader::saveAndLoad(JsonObject& inObj, FileMode mode) {
    JsonHelper::setStringArray(mGameObjectNames, "gameObjectNames", inObj);

    if (mode == FileMode::LOAD) {
        for (const auto& name : mGameObjectNames) {
            GameObjectCreater::create(name);
        }
    }
}

void GameObjectSaveAndLoader::addSaveGameObject(const std::string& name) {
    mGameObjectNames.emplace_back(name);
}
