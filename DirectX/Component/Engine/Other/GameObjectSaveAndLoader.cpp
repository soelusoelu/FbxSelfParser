#include "GameObjectSaveAndLoader.h"
#include "../../../GameObject/GameObjectFactory.h"
#include "../../../Utility/JsonHelper.h"

GameObjectSaveAndLoader::GameObjectSaveAndLoader()
    : Component()
{
}

GameObjectSaveAndLoader::~GameObjectSaveAndLoader() = default;

void GameObjectSaveAndLoader::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::setStringArray(mGameObjectNames, "gameObjectNames", inObj, alloc);

    if (mode == FileMode::LOAD) {
        for (const auto& name : mGameObjectNames) {
            GameObjectCreater::create(name);
        }
    }
}

void GameObjectSaveAndLoader::addSaveGameObject(const std::string& name) {
    mGameObjectNames.emplace_back(name);
}
