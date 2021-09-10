#include "AABB.h"
#include "BoxConstantGroup.h"
#include <array>

AABB::AABB() :
    min(Vector3::one * Math::infinity),
    max(Vector3::one * Math::negInfinity) {
}

AABB::AABB(const Vector3& min, const Vector3& max) :
    min(min),
    max(max) {
}

void AABB::updateMinMax(const Vector3& point) {
    min = Vector3::Min(min, point);
    max = Vector3::Max(max, point);
}

void AABB::updateMinMax(const AABB& src) {
    min = Vector3::Min(min, src.min);
    max = Vector3::Max(max, src.max);
}

void AABB::rotate(const Quaternion& q) {
    //ボックスの角の8つの点を格納する
    std::array<Vector3, 8> points;
    //最小点からボックスの角の点を計算していく
    points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT] = min;
    points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT] = Vector3(max.x, min.y, min.z);
    points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT] = Vector3(min.x, max.y, min.z);
    points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT] = Vector3(min.x, min.y, max.z);
    points[BoxConstantGroup::BOX_FORE_TOP_LEFT] = Vector3(min.x, max.y, max.z);
    points[BoxConstantGroup::BOX_FORE_TOP_RIGHT] = Vector3(max.x, min.y, max.z);
    points[BoxConstantGroup::BOX_BACK_TOP_LEFT] = Vector3(max.x, max.y, min.z);
    points[BoxConstantGroup::BOX_BACK_TOP_RIGHT] = max;

    //最初に最小の点を回転させる
    auto p = Vector3::transform(points[0], q);
    //最小、最大点を回転した点に設定
    min = p;
    max = p;

    //回転した点に基づいて最小、最大点を更新する
    for (size_t i = 1; i < points.size(); ++i) {
        p = Vector3::transform(points[i], q);
        updateMinMax(p);
    }
}

bool AABB::contains(const Vector3& point) const {
    bool outside = (
        point.x < min.x ||
        point.y < min.y ||
        point.z < min.z ||
        point.x > max.x ||
        point.y > max.y ||
        point.z > max.z
        );
    //いずれにも当てはまらなければ内側
    return !outside;
}

float AABB::minDistanceSquare(const Vector3& point) const {
    //各軸の差を計算する
    float dx = Math::Max(min.x - point.x, 0.f);
    dx = Math::Max(dx, point.x - max.x);
    float dy = Math::Max(min.y - point.y, 0.f);
    dy = Math::Max(dy, point.y - max.y);
    float dz = Math::Max(min.z - point.z, 0.f);
    dz = Math::Max(dy, point.z - max.z);
    //距離の2乗
    return (dx * dx + dy * dy + dz * dz);
}
