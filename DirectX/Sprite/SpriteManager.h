#pragma once

#include "../Math/Math.h"
#include <memory>
#include <list>

class Sprite;
class Sprite3D;
class SpriteComponent;

class SpriteManager {
    using SpritePtr = std::shared_ptr<Sprite>;
    using SpritePtrList = std::list<SpritePtr>;
    using SpriteComponentPtr = std::shared_ptr<SpriteComponent>;
    using SpriteComponentPtrList = std::list<SpriteComponentPtr>;
    using Sprite3DPtr = std::shared_ptr<Sprite3D>;
    using Sprite3DPtrList = std::list<Sprite3DPtr>;

public:
    SpriteManager();
    ~SpriteManager();
    void update();
    void draw(const Matrix4& proj) const;
    void drawComponents(const Matrix4& proj) const;
    void draw3Ds(const Matrix4& view, const Matrix4& proj) const;
    void add(const SpritePtr& sprite);
    void addComponent(const SpriteComponentPtr& add);
    void add3D(const Sprite3DPtr& add);
    void clear();
    //drawOrder順にソートする
    void sort();

private:
    void remove();

    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;

private:
    SpritePtrList mSprites;
    SpriteComponentPtrList mSpriteComponents;
    Sprite3DPtrList mSprite3Ds;
};
