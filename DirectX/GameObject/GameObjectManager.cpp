#include "GameObjectManager.h"
#include "GameObject.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

GameObjectManager::GameObjectManager(bool forGame)
    : mForGame(forGame)
{
    //ゲームシーン用のマネージャーなら
    if (forGame) {
        GameObject::setGameObjectManager(this);
    }
}

GameObjectManager::~GameObjectManager() {
    if (mForGame) {
        GameObject::setGameObjectManager(nullptr);
    }
}

const GameObjectPtrList& GameObjectManager::getGameObjects() const {
    return mGameObjects;
}

void GameObjectManager::add(const GameObjectPtr& newGameObject) {
    setNameNumber(*newGameObject);
    mGameObjects.emplace_back(newGameObject);
}

void GameObjectManager::update() {
    for (const auto& gameObject : mGameObjects) {
        gameObject->update();
    }
    for (const auto& gameObject : mGameObjects) {
        gameObject->lateUpdate();
    }

    remove();
}

void GameObjectManager::clear(const std::unordered_set<std::string>& tags) {
    auto itr = mGameObjects.begin();
    while (itr != mGameObjects.end()) {
        if (tags.find((*itr)->tag()) == tags.end()) {
            itr = mGameObjects.erase(itr);
        } else {
            ++itr;
        }
    }
}

const std::shared_ptr<GameObject>& GameObjectManager::find(const std::string& tag) const {
    for (const auto& gameObject : mGameObjects) {
        if (!gameObject->getActive()) {
            continue;
        }
        if (gameObject->tag() == tag) {
            return gameObject;
        }
    }
    //最後まで見つからなければnullptrを返す
    return nullptr;
}

std::vector<std::shared_ptr<GameObject>> GameObjectManager::findGameObjects(const std::string& tag) const {
    GameObjectPtrArray gameObjectArray;
    for (const auto& gameObject : mGameObjects) {
        if (!gameObject->getActive()) {
            continue;
        }
        if (gameObject->tag() == tag) {
            gameObjectArray.emplace_back(gameObject);
        }
    }

    return gameObjectArray;
}

void GameObjectManager::remove() {
    auto itr = mGameObjects.begin();
    while (itr != mGameObjects.end()) {
        if ((*itr)->isDead()) {
            itr = mGameObjects.erase(itr);
        } else {
            ++itr;
        }
    }
}

void GameObjectManager::setNameNumber(GameObject& target) const {
    std::string name = target.name();
    bool isEnd = false;
    checkNameNumber(name, isEnd);
    target.setName(name);
}

void GameObjectManager::checkNameNumber(std::string& name, bool& isEnd, int number) const {
    std::string numberedName = name;
    //再帰してたら名前の後ろに数字をつける
    if (number > 0) {
        numberedName += StringUtil::intToString(number);
    }
    for (const auto& obj : mGameObjects) {
        if (obj->name() == numberedName) {
            //名前が一致するゲームオブジェクトが存在したら、末尾の数字を1増やして再帰する
            ++number;
            checkNameNumber(name, isEnd, number);
        }
    }

    if (isEnd) {
        return;
    }
    name = numberedName;
    isEnd = true;
}
