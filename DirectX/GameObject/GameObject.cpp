#include "GameObject.h"
#include "GameObjectManager.h"
#include "../Transform/Transform3D.h"
#include "../Component/ComponentManager.h"

GameObject::GameObject()
    : mTransform(std::make_unique<Transform3D>(this))
    , mComponentManager(std::make_unique<ComponentManager>())
    , mName()
    , mTag()
    , mIsActive(true)
{
}

GameObject::~GameObject() {
    mComponentManager->finalize();
}

GameObject& GameObject::getThis() {
    return *this;
}

const GameObject& GameObject::getThis() const {
    return *this;
}

void GameObject::update() {
    if (getActive()) {
        mComponentManager->update();
    }
    updateDestroyTimer();
}

void GameObject::lateUpdate() {
    if (getActive()) {
        mComponentManager->lateUpdate();

        mComponentManager->destroy();

        mTransform->computeMatrix();
    }
}

void GameObject::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mTransform->saveAndLoad(inObj, alloc, mode);
}

void GameObject::setActive(bool value) {
    mIsActive = value;

    mComponentManager->onEnable(value);
}

bool GameObject::getActive() const {
    return mIsActive;
}

void GameObject::setName(const std::string& name) {
    mName = name;
}

const std::string& GameObject::name() const {
    return mName;
}

void GameObject::setTag(const std::string& tag) {
    mTag = tag;
}

const std::string& GameObject::tag() const {
    return mTag;
}

Transform3D& GameObject::transform() const {
    return *mTransform;
}

ComponentManager& GameObject::componentManager() const {
    return *mComponentManager;
}

GameObjectManager& GameObject::getGameObjectManager() const {
    return *mGameObjectManager;
}

void GameObject::setGameObjectManager(GameObjectManager* manager) {
    mGameObjectManager = manager;
}

std::shared_ptr<GameObject> GameObject::create(const std::string& name, const std::string& tag) {
    auto obj = std::make_shared<GameObject>();
    //名前を設定
    obj->mName = name;
    //タグを設定
    obj->mTag = tag;
    //マネージャーに登録
    if (mGameObjectManager) {
        mGameObjectManager->add(obj);
    }

    return obj;
}
