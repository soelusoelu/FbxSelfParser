#include "AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Device/Physics.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Imgui/imgui.h"
#include "../../../Transform/ParentChildRelationship.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

AABBCollider::AABBCollider()
    : Collider()
    , mAABB()
    , mDefaultMin(Vector3::zero)
    , mDefaultMax(Vector3::zero)
    , mIsRenderCollision(true)
    , mLoadedProperties(false)
{
}

AABBCollider::~AABBCollider() = default;

ColliderType AABBCollider::getType() const {
    return ColliderType::AABB;
}

void AABBCollider::start() {
    Collider::start();

    //ファイルから値を読み込んでいないなら頂点から形成する
    if (!mLoadedProperties) {
        create();
    }

    //早速transformが変わっているかもしれないから更新する
    updateAABB();
    //最新のAABBの点を計算する
    updatePoints();

    //if (mPhysics) {
    //    mPhysics->add(shared_from_this());
    //}
}

void AABBCollider::lateUpdate() {
    Collider::lateUpdate();

    //当たり判定が自動化設定されているなら
    if (mIsAutoUpdate) {
        updateAABB();
    }

    //AABBの点を更新する
    updatePoints();

    //当たり判定を可視化する
    renderCollision();
}

void AABBCollider::finalize() {
    Collider::finalize();

    //if (mPhysics) {
    //    mPhysics->remove(shared_from_this());
    //}
}

void AABBCollider::onEnable(bool value) {
    Collider::onEnable(value);

    setRenderCollision(value);
}

void AABBCollider::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    Collider::saveAndLoad(inObj, alloc, mode);

    if (mode == FileMode::SAVE) {
        JsonHelper::setVector3(mDefaultMin, "min", inObj, alloc);
        JsonHelper::setVector3(mDefaultMax, "max", inObj, alloc);
    } else {
        if (JsonHelper::getVector3(mDefaultMin, "min", inObj)) {
            mAABB.min = mDefaultMin;
            mLoadedProperties = true;
        }
        if (JsonHelper::getVector3(mDefaultMax, "max", inObj)) {
            mAABB.max = mDefaultMax;
            mLoadedProperties = true;
        }
    }
    JsonHelper::getSet(mIsRenderCollision, "isRenderCollision", inObj, alloc, mode);
}

void AABBCollider::drawInspector() {
    Collider::drawInspector();

    ImGuiWrapper::dragVector3("DefaultMin", mDefaultMin, 0.01f);
    ImGuiWrapper::dragVector3("DefaultMax", mDefaultMax, 0.01f);
    ImGui::Checkbox("IsRenderCollision", &mIsRenderCollision);
}

void AABBCollider::set(const Vector3& min, const Vector3& max) {
    mAABB.min = min;
    mAABB.max = max;

    //デフォルト点を修正する
    computeDefaultPoint();

    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const AABB& AABBCollider::getAABB() const {
    return mAABB;
}

const BoxPoints& AABBCollider::getBoxPoints() const {
    return mPoints;
}

std::array<std::pair<Vector3, Vector3>, BoxConstantGroup::SURFACES_NUM> AABBCollider::getBoxSurfacesCenterAndNormal() const {
    return std::array<std::pair<Vector3, Vector3>, BoxConstantGroup::SURFACES_NUM> {
        //面の中心位置と法線を格納する
        std::make_pair((mPoints[3] + mPoints[0]) / 2.f, Vector3::down),
        std::make_pair((mPoints[7] + mPoints[4]) / 2.f, Vector3::up),
        std::make_pair((mPoints[3] + mPoints[5]) / 2.f, Vector3::right),
        std::make_pair((mPoints[2] + mPoints[4]) / 2.f, Vector3::left),
        std::make_pair((mPoints[5] + mPoints[0]) / 2.f, Vector3::back),
        std::make_pair((mPoints[7] + mPoints[2]) / 2.f, Vector3::forward)
    };
}

void AABBCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void AABBCollider::create() {
    auto meshComponent = getComponent<MeshComponent>();
    if (meshComponent) {
        const auto mesh = meshComponent->getMesh();
        //メッシュ情報からAABBを作成する
        createAABB(*mesh);
    }
}

void AABBCollider::createAABB(const IMesh& mesh) {
    //すべてのメッシュからAABBを作成する
    for (size_t i = 0; i < mesh.getMeshCount(); ++i) {
        Vector3 min, max;
        computeMinMax(min, max, mesh.getMeshVertices(i));

        //当たり判定更新
        mAABB.updateMinMax(min);
        mAABB.updateMinMax(max);
    }
    mDefaultMin = mAABB.min;
    mDefaultMax = mAABB.max;
}

void AABBCollider::computeMinMax(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices) {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;

    //メッシュ情報から最小、最大点を割り出す
    for (size_t i = 0; i < meshVertices.size(); ++i) {
        const auto& vertices = meshVertices[i];
        const auto& p = vertices.pos;
        if (p.x < min.x) {
            min.x = p.x;
        }
        if (p.x > max.x) {
            max.x = p.x;
        }
        if (p.y < min.y) {
            min.y = p.y;
        }
        if (p.y > max.y) {
            max.y = p.y;
        }
        if (p.z < min.z) {
            min.z = p.z;
        }
        if (p.z > max.z) {
            max.z = p.z;
        }
    }

    outMin = min;
    outMax = max;
}

void AABBCollider::updateAABB() {
    const auto& t = transform();
    const auto& pos = t.getPosition();
    const auto& scale = t.getScale();
    //回転を適用するために一時的なAABBを作成する
    AABB aabb((mDefaultMin - t.getPivot()) * scale, (mDefaultMax - t.getPivot()) * scale);
    //回転を適用する
    aabb.rotate(t.getRotation());
    //既存のAABBを更新する
    mAABB.min = aabb.min + pos;
    mAABB.max = aabb.max + pos;
}

void AABBCollider::updatePoints() {
    const auto& min = mAABB.min;
    const auto& max = mAABB.max;
    mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT] = min;
    mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT] = Vector3(max.x, min.y, min.z);
    mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT] = Vector3(min.x, min.y, max.z);
    mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT] = Vector3(max.x, min.y, max.z);
    mPoints[BoxConstantGroup::BOX_FORE_TOP_LEFT] = Vector3(min.x, max.y, min.z);
    mPoints[BoxConstantGroup::BOX_FORE_TOP_RIGHT] = Vector3(max.x, max.y, min.z);
    mPoints[BoxConstantGroup::BOX_BACK_TOP_LEFT] = Vector3(min.x, max.y, max.z);
    mPoints[BoxConstantGroup::BOX_BACK_TOP_RIGHT] = max;
}

void AABBCollider::computeDefaultPoint() {
    const auto& t = transform();
    const auto& pos = t.getPosition();
    const auto& scale = t.getScale();

    //updateAABBの逆順に計算していく
    auto min = Vector3(
        mAABB.min.x / scale.x - pos.x,
        mAABB.min.y / scale.y - pos.y,
        mAABB.min.z / scale.z - pos.z
    );
    auto max = Vector3(
        mAABB.max.x / scale.x - pos.x,
        mAABB.max.y / scale.y - pos.y,
        mAABB.max.z / scale.z - pos.z
    );

    //更新する
    mDefaultMin = min + t.getPivot();
    mDefaultMax = max + t.getPivot();
}

void AABBCollider::renderCollision() const {
#ifdef _DEBUG
    if (!mIsRenderCollision) {
        return;
    }
    if (!mEnable) {
        return;
    }

    //デバッグ時のみ当たり判定を表示
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT], mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT], mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT], mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], ColorPalette::lightGreen);

    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_TOP_LEFT], mPoints[BoxConstantGroup::BOX_FORE_TOP_RIGHT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_TOP_LEFT], mPoints[BoxConstantGroup::BOX_BACK_TOP_LEFT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_BACK_TOP_LEFT], mPoints[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_TOP_RIGHT], mPoints[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);

    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT], mPoints[BoxConstantGroup::BOX_FORE_TOP_LEFT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT], mPoints[BoxConstantGroup::BOX_FORE_TOP_RIGHT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], mPoints[BoxConstantGroup::BOX_BACK_TOP_LEFT], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], mPoints[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);
#endif // _DEBUG
}
