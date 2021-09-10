#include "SpriteManager.h"
#include "Sprite.h"
#include "../Component/Engine/Sprite/Sprite3D.h"
#include "../Component/Engine/Sprite/SpriteComponent.h"

SpriteManager::SpriteManager() {
    Sprite::setSpriteManager(this);
    SpriteComponent::setSpriteManager(this);
    Sprite3D::setSpriteManager(this);
}

SpriteManager::~SpriteManager() {
    Sprite::setSpriteManager(nullptr);
    SpriteComponent::setSpriteManager(nullptr);
    Sprite3D::setSpriteManager(nullptr);
}

void SpriteManager::update() {
    remove();

    for (const auto& sprite : mSprites) {
        if (sprite->getActive()) {
            sprite->computeWorldTransform();
        }
    }
}

void SpriteManager::draw(const Matrix4& proj) const {
    if (mSprites.empty()) {
        return;
    }

    for (const auto& sprite : mSprites) {
        if (!sprite->getActive() || sprite->isDead()) {
            continue;
        }

        //描画する
        sprite->draw(proj);
    }
}

void SpriteManager::drawComponents(const Matrix4& proj) const {
    if (mSpriteComponents.empty()) {
        return;
    }

    for (const auto& sprite : mSpriteComponents) {
        if (!sprite->getActive() || sprite->isDead()) {
            continue;
        }

        //描画する
        sprite->draw(proj);
    }
}

void SpriteManager::draw3Ds(const Matrix4& view, const Matrix4& proj) const {
    if (mSprite3Ds.empty()) {
        return;
    }

    auto cancelRotation = view;
    cancelRotation.m[3][0] = 0.f;
    cancelRotation.m[3][1] = 0.f;
    cancelRotation.m[3][2] = 0.f;
    cancelRotation.inverse();

    for (const auto& sprite : mSprite3Ds) {
        if (!sprite->getActive() || sprite->isDead()) {
            continue;
        }

        if (sprite->isBillboard()) {
            sprite->drawBillboard(cancelRotation, view * proj);
        } else {
            sprite->draw(view * proj);
        }
    }
}

void SpriteManager::add(const SpritePtr& sprite) {
    mSprites.emplace_back(sprite);
}

void SpriteManager::addComponent(const SpriteComponentPtr& add) {
    int order = add->getDrawOrder();
    auto itr = mSpriteComponents.begin();
    for (; itr != mSpriteComponents.end(); ++itr) {
        if (order < (*itr)->getDrawOrder()) {
            break;
        }
    }
    mSpriteComponents.insert(itr, add);
}

void SpriteManager::add3D(const Sprite3DPtr& add) {
    mSprite3Ds.emplace_back(add);
}

void SpriteManager::clear() {
    remove();
}

void SpriteManager::sort() {
    mSpriteComponents.sort(SpriteComponent::compare);
}

void SpriteManager::remove() {
    auto itr = mSpriteComponents.begin();
    while (itr != mSpriteComponents.end()) {
        if ((*itr)->isDead()) {
            itr = mSpriteComponents.erase(itr);
        } else {
            ++itr;
        }
    }

    auto itr2 = mSprite3Ds.begin();
    while (itr2 != mSprite3Ds.end()) {
        if ((*itr2)->isDead()) {
            itr2 = mSprite3Ds.erase(itr2);
        } else {
            ++itr2;
        }
    }
}
