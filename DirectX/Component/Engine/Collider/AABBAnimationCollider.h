#pragma once

#include "Collider.h"
#include "../Mesh/IAnimationCPU.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include <memory>
#include <vector>

class MeshComponent;
class AnimationCPU;

//AABBが持つ情報群
struct AABBInfomation {
    //AABB
    AABB aabb;
    //AABBの結合対象
    std::vector<unsigned> concatenateTargets;
    //AABBの各点
    BoxPoints points;
    //アクティブか
    bool isActive = true;
};

using AABBs = std::vector<AABBInfomation>;

class AABBAnimationCollider
    : public Collider
    , public std::enable_shared_from_this<AABBAnimationCollider>
{
public:
    AABBAnimationCollider();
    ~AABBAnimationCollider();
    virtual ColliderType getType() const override;

    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void drawInspector() override;

    //指定したAABBを結合する
    void concatenate(unsigned a, unsigned b);
    //全AABBを取得する
    const AABBs& getAABBs() const;
    //AABBを取得する
    const AABB& getAABB(unsigned index) const;
    //当たり判定を可視化するか
    void setRenderCollision(bool value);

private:
    AABBAnimationCollider(const AABBAnimationCollider&) = delete;
    AABBAnimationCollider& operator=(const AABBAnimationCollider&) = delete;

    //AABBを更新する
    void computeAABB();
    //AABBのサイズを更新する
    void updateAABB(unsigned target, unsigned index);
    //AABBを作成する
    void createAABB(unsigned index);
    //transformを考慮したAABBにする
    void adaptAABBToTransform(unsigned index);
    //AABBを生成する
    AABB createdAABB(unsigned index) const;
    //AABBの点を更新する
    void updatePoints();
    //当たり判定を可視化する
    void renderCollision();

private:
    //当たり判定であるAABB情報群
    AABBs mAABBs;
    //メッシュコンポーネント
    std::shared_ptr<MeshComponent> mMesh;
    //アニメーションコンポーネント
    std::shared_ptr<AnimationCPU> mAnimationCPU;
    //当たり判定を表示するか
    bool mIsRenderCollision;
};
