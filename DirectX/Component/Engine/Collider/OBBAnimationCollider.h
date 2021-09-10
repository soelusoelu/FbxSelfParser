#pragma once

#include "Collider.h"
#include "../Mesh/IAnimationCPU.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include <memory>
#include <vector>

class MeshComponent;
class AnimationCPU;

//OBBが持つ情報群
struct OBBInfomation {
    //OBB
    OBB obb;
    //OBBの結合対象
    std::vector<unsigned> concatenateTargets;
    //アクティブか
    bool isActive = true;
};

class OBBAnimationCollider
    : public Collider
{
public:
    OBBAnimationCollider();
    ~OBBAnimationCollider();
    virtual ColliderType getType() const override;

    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void drawInspector() override;

    //指定したOBBを結合する
    void concatenate(unsigned a, unsigned b);
    //OBBを取得する
    const OBB& getOBB(unsigned index) const;
    //当たり判定を可視化するか
    void setRenderCollision(bool value);

private:
    OBBAnimationCollider(const OBBAnimationCollider&) = delete;
    OBBAnimationCollider& operator=(const OBBAnimationCollider&) = delete;

    //OBBを更新する
    void computeOBB();
    //OBBのサイズを更新する
    void updateOBB(unsigned target, unsigned index);
    //OBBを作成する
    void createOBB(unsigned meshIndex);
    //当たり判定を可視化する
    void renderCollision();

private:
    //当たり判定であるOBB情報群
    std::vector<OBBInfomation> mOBBs;
    //メッシュコンポーネント
    std::shared_ptr<MeshComponent> mMesh;
    //アニメーションコンポーネント
    std::shared_ptr<AnimationCPU> mAnimationCPU;
    //当たり判定を表示するか
    bool mIsRenderCollision;
};
