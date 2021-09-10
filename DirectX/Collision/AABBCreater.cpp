#include "AABBCreater.h"
#include "Collision.h"

AABB AABBCreater::create(const std::vector<Vector3>& positions) {
    auto min = Vector3::one * FLT_MAX;
    auto max = Vector3::one * FLT_MIN;

    //メッシュ情報から最小、最大点を割り出す
    for (const auto& p : positions) {
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

    return AABB(min, max);
}
