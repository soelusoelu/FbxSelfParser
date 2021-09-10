#pragma once

#include "../Mesh/IMesh.h"

struct Sphere;

class SphereHelper {
public:
    //メッシュからスフィアを形成する
    static void create(Sphere& out, const IMesh& mesh);

private:
    SphereHelper() = delete;
    ~SphereHelper() = delete;

    //最小・最大頂点を求める
    static void computeMinMaxPoint(Vector3& outMin, Vector3& outMax, const MeshVertices& meshVertices);
};
