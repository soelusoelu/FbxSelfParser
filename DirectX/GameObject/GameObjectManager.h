#pragma once

#include "IGameObjectAdder.h"
#include "IGameObjectsGetter.h"
#include <list>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class GameObjectManager
    : public IGameObjectsGetter
    , public IGameObjectAdder
{
    using GameObjectPtrArray = std::vector<GameObjectPtr>;

public:
    GameObjectManager(bool forGame = true);
    ~GameObjectManager();
    //全ゲームオブジェクトを取得する
    virtual const GameObjectPtrList& getGameObjects() const override;
    //ゲームオブジェクトの登録
    virtual void add(const GameObjectPtr& newGameObject) override;
    //登録済みの全ゲームオブジェクトの更新
    void update();
    //登録済みの全ゲームオブジェクトの削除
    void clear(const std::unordered_set<std::string>& tags);
    //tagに一致するアクティブなゲームオブジェクトの検索
    const GameObjectPtr& find(const std::string& tag) const;
    //tagに一致するアクティブな全ゲームオブジェクトの検索
    GameObjectPtrArray findGameObjects(const std::string& tag) const;

private:
    //コピー禁止
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

    //ゲームオブジェクトの削除
    void remove();
    //ゲームオブジェクトの名前がかぶらないように番号で調整する
    void setNameNumber(GameObject& target) const;
    //ゲームオブジェクトの名前を走査していく
    void checkNameNumber(std::string& name, bool& isEnd, int number = 0) const;

private:
    //ゲームオブジェクトリスト
    GameObjectPtrList mGameObjects;
    //ゲームシーン用のマネージャーか
    bool mForGame;
};
