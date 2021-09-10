#include "SphereHelper.h"
#include "Sphere.h"
#include "../Math/Math.h"

void SphereHelper::create(Sphere& out, const IMesh& mesh) {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < mesh.getMeshCount(); i++) {
        Vector3 tempMin, tempMax;
        computeMinMaxPoint(tempMin, tempMax, mesh.getMeshVertices(i));
        min = Vector3::Min(min, tempMin);
        max = Vector3::Max(max, tempMax);
    }
    out.center = (min + max) / 2.f;
    out.radius = (max - min).length() / 2.f;
}

void SphereHelper::computeMinMaxPoint(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices) {
    //中心位置計算のための最小、最大位置
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
