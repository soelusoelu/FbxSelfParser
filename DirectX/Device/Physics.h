#pragma once

#include <memory>
#include <vector>

class Collider;

class Physics {
    using ColliderPtr = std::shared_ptr<Collider>;
    using ColliderPtrArray = std::vector<ColliderPtr>;

public:
    Physics();
    ~Physics();
    //コライダーの追加・削除
    void add(const ColliderPtr& collider);
    void remove(const ColliderPtr& collider);
    //全削除
    void clear();
    //総当たり判定
    void sweepAndPrune();

private:
    //コライダーが有効か
    bool enabled(const Collider& collider);

private:
    ColliderPtrArray mColliders;
};
