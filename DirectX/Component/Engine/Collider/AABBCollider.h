#pragma once

#include "Collider.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IMesh.h"
#include "../../../Mesh/IMeshLoader.h"
#include <array>
#include <memory>
#include <utility>

class AABBCollider
    : public Collider
    , public std::enable_shared_from_this<AABBCollider>
{
public:
    AABBCollider();
    ~AABBCollider();
    virtual ColliderType getType() const override;

    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    //AABBの最小と最大点を直接設定する
    void set(const Vector3& min, const Vector3& max);
    //AABBを取得する
    const AABB& getAABB() const;
    //AABBのすべての点を取得する
    const BoxPoints& getBoxPoints() const;
    //AABBのすべての面の中心位置と法線を取得する
    std::array<std::pair<Vector3, Vector3>, BoxConstantGroup::SURFACES_NUM> getBoxSurfacesCenterAndNormal() const;
    //当たり判定を可視化するか
    void setRenderCollision(bool value);

private:
    AABBCollider(const AABBCollider&) = delete;
    AABBCollider& operator=(const AABBCollider&) = delete;

    //AABBを作成する
    void create();
    //メッシュ情報からAABBを作成する
    void createAABB(const IMesh& mesh);
    //メッシュから最小、最大点を割り出す
    void computeMinMax(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices);
    //AABBを更新する
    void updateAABB();
    //AABBの点を更新する
    void updatePoints();
    //最新の点からtransformを掛ける前の点を求める
    void computeDefaultPoint();
    //当たり判定を可視化する
    void renderCollision() const;

private:
    //当たり判定であるAABB
    AABB mAABB;
    //transformの影響を考慮しない最小点
    Vector3 mDefaultMin;
    //transformの影響を考慮しない最大点
    Vector3 mDefaultMax;
    //AABBの各点
    BoxPoints mPoints;
    //当たり判定を表示するか
    bool mIsRenderCollision;
    //ファイルから値を読み込んだか
    bool mLoadedProperties;
};
