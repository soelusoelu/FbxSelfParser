#include "Intersect.h"
#include "AABB.h"
#include "BoxHelper.h"
#include "Circle.h"
#include "IntersectHelper.h"
#include "OBB.h"
#include "Ray.h"
#include "RaycastHit.h"
#include "Sphere.h"
#include "Square.h"
#include "Triangle.h"
#include "../Component/Engine/Mesh/MeshComponent.h"
#include "../Component/Engine/Mesh/MeshRenderer.h"
#include "../Mesh/IMesh.h"
#include "../Transform/Transform3D.h"
#include <algorithm>
#include <vector>

bool Intersect::intersectCircle(const Circle& a, const Circle& b) {
    Vector2 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadius = a.radius + b.radius;
    return distSq <= (sumRadius * sumRadius);
}

bool Intersect::intersectSphere(const Sphere& a, const Sphere& b) {
    Vector3 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadii = a.radius + b.radius;
    return distSq <= (sumRadii * sumRadii);
}

bool Intersect::intersectAABB(const AABB& a, const AABB& b) {
    bool no = (
        a.max.x < b.min.x ||
        a.max.y < b.min.y ||
        a.max.z < b.min.z ||
        b.max.x < a.min.x ||
        b.max.y < a.min.y ||
        b.max.z < a.min.z
        );
    //いずれにも当てはまらなければ衝突している
    return !no;
}

//cpp only
//分離軸に投影された軸成分から投影線分長を算出
float lengthSegmentOnSeparateAxis(const Vector3& sep, const Vector3& e1, const Vector3& e2, const Vector3* e3 = nullptr) {
    //3つの内積の絶対値の和で投影線分長を計算
    //分離軸sepは標準化されていること
    float r1 = Math::abs(Vector3::dot(sep, e1));
    float r2 = Math::abs(Vector3::dot(sep, e2));
    float r3 = e3 ? (Math::abs(Vector3::dot(sep, *e3))) : 0.f;
    return r1 + r2 + r3;
}

bool Intersect::intersectOBB(const OBB& a, const OBB& b) {
    //各方向ベクトル
    auto NAe1 = Vector3::transform(Vector3::right, a.rotation);
    auto Ae1 = NAe1 * a.extents.x;
    auto NAe2 = Vector3::transform(Vector3::up, a.rotation);
    auto Ae2 = NAe2 * a.extents.y;
    auto NAe3 = Vector3::transform(Vector3::forward, a.rotation);
    auto Ae3 = NAe3 * a.extents.z;
    auto NBe1 = Vector3::transform(Vector3::right, b.rotation);
    auto Be1 = NBe1 * b.extents.x;
    auto NBe2 = Vector3::transform(Vector3::up, b.rotation);
    auto Be2 = NBe2 * b.extents.y;
    auto NBe3 = Vector3::transform(Vector3::forward, b.rotation);
    auto Be3 = NBe3 * b.extents.z;
    //OBB間の距離
    auto interval = a.center - b.center;

    //分離軸 : Ae1
    float rA = Ae1.length();
    float rB = lengthSegmentOnSeparateAxis(NAe1, Be1, Be2, &Be3);
    float L = Math::abs(Vector3::dot(interval, NAe1));
    if (L > rA + rB) {
        return false; // 衝突していない
    }

     //分離軸 : Ae2
    rA = Ae2.length();
    rB = lengthSegmentOnSeparateAxis(NAe2, Be1, Be2, &Be3);
    L = Math::abs(Vector3::dot(interval, NAe2));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : Ae3
    rA = Ae3.length();
    rB = lengthSegmentOnSeparateAxis(NAe3, Be1, Be2, &Be3);
    L = Math::abs(Vector3::dot(interval, NAe3));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : Be1
    rA = lengthSegmentOnSeparateAxis(NBe1, Ae1, Ae2, &Ae3);
    rB = Be1.length();
    L = Math::abs(Vector3::dot(interval, NBe1));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : Be2
    rA = lengthSegmentOnSeparateAxis(NBe2, Ae1, Ae2, &Ae3);
    rB = Be2.length();
    L = Math::abs(Vector3::dot(interval, NBe2));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : Be3
    rA = lengthSegmentOnSeparateAxis(NBe3, Ae1, Ae2, &Ae3);
    rB = Be3.length();
    L = Math::abs(Vector3::dot(interval, NBe3));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C11
    auto cross = Vector3::cross(NAe1, NBe1);
    rA = lengthSegmentOnSeparateAxis(cross, Ae2, Ae3);
    rB = lengthSegmentOnSeparateAxis(cross, Be2, Be3);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C12
    cross = Vector3::cross(NAe1, NBe2);
    rA = lengthSegmentOnSeparateAxis(cross, Ae2, Ae3);
    rB = lengthSegmentOnSeparateAxis(cross, Be1, Be3);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C13
    cross = Vector3::cross(NAe1, NBe3);
    rA = lengthSegmentOnSeparateAxis(cross, Ae2, Ae3);
    rB = lengthSegmentOnSeparateAxis(cross, Be1, Be2);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C21
    cross = Vector3::cross(NAe2, NBe1);
    rA = lengthSegmentOnSeparateAxis(cross, Ae1, Ae3);
    rB = lengthSegmentOnSeparateAxis(cross, Be2, Be3);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C22
    cross = Vector3::cross(NAe2, NBe2);
    rA = lengthSegmentOnSeparateAxis(cross, Ae1, Ae3);
    rB = lengthSegmentOnSeparateAxis(cross, Be1, Be3);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C23
    cross = Vector3::cross(NAe2, NBe3);
    rA = lengthSegmentOnSeparateAxis(cross, Ae1, Ae3);
    rB = lengthSegmentOnSeparateAxis(cross, Be1, Be2);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C31
    cross = Vector3::cross(NAe3, NBe1);
    rA = lengthSegmentOnSeparateAxis(cross, Ae1, Ae2);
    rB = lengthSegmentOnSeparateAxis(cross, Be2, Be3);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C32
    cross = Vector3::cross(NAe3, NBe2);
    rA = lengthSegmentOnSeparateAxis(cross, Ae1, Ae2);
    rB = lengthSegmentOnSeparateAxis(cross, Be1, Be3);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離軸 : C33
    cross = Vector3::cross(NAe3, NBe3);
    rA = lengthSegmentOnSeparateAxis(cross, Ae1, Ae2);
    rB = lengthSegmentOnSeparateAxis(cross, Be1, Be2);
    L = Math::abs(Vector3::dot(interval, cross));
    if (L > rA + rB) {
        return false;
    }

    //分離平面が存在しないから衝突している
    return true;
}

bool Intersect::intersectRayPlane(const Ray& ray, const Plane& p, Vector3* intersectPoint) {
    //tの解決策があるかどうかの最初のテスト
    float denom = Vector3::dot(ray.end - ray.start, p.normal());
    if (Math::nearZero(denom)) {
        //交差するのは、開始が平面上の点(P dot N) == dである場合だけ
        return (Math::nearZero(Vector3::dot(ray.start, p.normal()) - p.d));
    }

    float numer = -Vector3::dot(ray.start, p.normal()) - p.d;
    float t = numer / denom;
    //tが線分の範囲内にあるか
    if (t >= 0.f && t <= 1.f) {
        //衝突点を取得する
        if (intersectPoint) {
            *intersectPoint = ray.pointOnSegment(t);
        }
        return true;
    }

    //衝突していない
    return false;
}

bool Intersect::intersectRayPolygon(const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector3* intersectPoint) {
    //まずは無限平面でテストする
    Plane plane(p0, p1, p2);
    Vector3 interPoint{};
    if (!intersectRayPlane(ray, plane, &interPoint)) {
        return false;
    }

    //各辺ベクトルを求める
    auto ab = p1 - p0;
    auto bc = p2 - p1;
    auto ca = p0 - p2;

    //各辺ベクトルと各頂点から交点へ向かうベクトルの外積を求める
    auto crossAB = Vector3::cross(ab, interPoint - p0);
    auto crossBC = Vector3::cross(bc, interPoint - p1);
    auto crossCA = Vector3::cross(ca, interPoint - p2);

    //それぞれの外積ベクトルとの内積を計算する
    auto dotAB = Vector3::dot(plane.normal(), crossAB);
    auto dotBC = Vector3::dot(plane.normal(), crossBC);
    auto dotCA = Vector3::dot(plane.normal(), crossCA);

    //3つの内積結果のうち1つでもマイナス符号のものがあれば交点は外にある
    if (!(dotAB < 0.f || dotBC < 0.f || dotCA < 0.f)) {
        if (intersectPoint) {
            *intersectPoint = interPoint;
        }
        return true;
    }

    return false;
}

bool Intersect::intersectRayPolygon(const Ray& ray, const Triangle& polygon, Vector3* intersectPoint) {
    return intersectRayPolygon(ray, polygon.p0, polygon.p1, polygon.p2, intersectPoint);
}

bool Intersect::intersectRaySphere(const Ray& ray, const Sphere& sphere, Vector3* intersectPoint) {
    //方程式のX, Y, a, b, cを計算
    Vector3 X = ray.start - sphere.center;
    Vector3 Y = ray.end - ray.start;
    float a = Vector3::dot(Y, Y);
    float b = 2.f * Vector3::dot(X, Y);
    float c = Vector3::dot(X, X) - sphere.radius * sphere.radius;
    //判別式を計算
    float disc = b * b - 4.f * a * c;
    if (disc < 0.f) {
        return false;
    }

    disc = Math::sqrt(disc);
    //tの解(minとmax)を求める
    float tMin = (-b - disc) / (2.f * a);
    float tMax = (-b + disc) / (2.f * a);
    //tが線分の領域にあるのかチェック
    if (tMin >= 0.f && tMin <= 1.f) {
        if (intersectPoint) {
            *intersectPoint = ray.pointOnSegment(tMin);
        }
        return true;
    } else if (tMax >= 0.f && tMax <= 1.f) {
        if (intersectPoint) {
            *intersectPoint = ray.pointOnSegment(tMax);
        }
        return true;
    }

    return false;
}

bool testSidePlane(float start, float end, float negd, std::vector<float>& out) {
    float denom = end - start;
    if (Math::nearZero(denom)) {
        return false;
    }

    float numer = -start + negd;
    float t = numer / denom;
    //tが線分の範囲内にあるか
    if (t >= 0.f && t <= 1.f) {
        out.emplace_back(t);
        return true;
    }

    //範囲外
    return false;
}

bool Intersect::intersectRayAABB(const Ray& ray, const AABB& aabb) {
    Vector3 temp;
    return intersectRayAABB(ray, aabb, temp);
}

bool Intersect::intersectRayAABB(const Ray& ray, const AABB& aabb, Vector3& intersectPoint) {
    //すべてのt値を格納する
    std::vector<float> tValues;
    //x平面テスト
    testSidePlane(ray.start.x, ray.end.x, aabb.min.x, tValues);
    testSidePlane(ray.start.x, ray.end.x, aabb.max.x, tValues);
    //y平面テスト
    testSidePlane(ray.start.y, ray.end.y, aabb.min.y, tValues);
    testSidePlane(ray.start.y, ray.end.y, aabb.max.y, tValues);
    //z平面テスト
    testSidePlane(ray.start.z, ray.end.z, aabb.min.z, tValues);
    testSidePlane(ray.start.z, ray.end.z, aabb.max.z, tValues);

    //t値を昇順で並べ替える
    std::sort(
        tValues.begin(),
        tValues.end(),
        [&](float a, float b) { return a < b; }
    );

    //ボックスに交点が含まれているか調べる
    for (const auto& t : tValues) {
        auto point = ray.pointOnSegment(t);
        if (aabb.contains(point)) {
            intersectPoint = point;
            return true;
        }
    }

    //衝突していない
    return false;
}

bool Intersect::intersectRayOBB(const Ray& ray, const OBB& obb) {
    auto points = BoxHelper::getPoints(obb);

    const auto& ftr = points[BoxConstantGroup::BOX_BACK_TOP_RIGHT];
    const auto& fbr = points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT];
    const auto& ftl = points[BoxConstantGroup::BOX_BACK_TOP_LEFT];
    const auto& fbl = points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT];
    const auto& btr = points[BoxConstantGroup::BOX_FORE_TOP_RIGHT];
    const auto& bbr = points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT];
    const auto& btl = points[BoxConstantGroup::BOX_FORE_TOP_LEFT];
    const auto& bbl = points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT];

    //各面からポリゴンの衝突判定を行う
    //上面
    if (intersectRayPolygon(ray, ftr, ftl, btr)) {
        return true;
    } else if (intersectRayPolygon(ray, ftl, btr, btl)) {
        return true;
    }
    //下面
    if (intersectRayPolygon(ray, fbr, fbl, bbr)) {
        return true;
    } else if (intersectRayPolygon(ray, fbl, bbr, bbl)) {
        return true;
    }
    //前面
    if (intersectRayPolygon(ray, btr, btl, bbr)) {
        return true;
    } else if (intersectRayPolygon(ray, btl, bbr, bbl)) {
        return true;
    }
    //奥面
    if (intersectRayPolygon(ray, ftr, ftl, fbr)) {
        return true;
    } else if (intersectRayPolygon(ray, ftl, fbr, fbl)) {
        return true;
    }
    //右面
    if (intersectRayPolygon(ray, ftr, btr, fbr)) {
        return true;
    } else if (intersectRayPolygon(ray, btr, fbr, bbr)) {
        return true;
    }
    //左面
    if (intersectRayPolygon(ray, ftl, btl, bbl)) {
        return true;
    } else if (intersectRayPolygon(ray, btl, bbl, fbl)) {
        return true;
    }

    return false;
}

bool Intersect::intersectRayMesh(const Ray& ray, const IMesh& mesh, const Transform3D& transform, RaycastHit* raycastHit) {
    //ワールド行列を先に取得しておく
    const auto& world = transform.getWorldTransform();

    //すべてのメッシュとレイによる判定を行う
    for (unsigned i = 0; i < mesh.getMeshCount(); ++i) {
        const auto& meshVertices = mesh.getMeshVertices(i);
        const auto& meshIndices = mesh.getMeshIndices(i);
        const auto polygonCount = mesh.getPolygonCount(i);
        for (unsigned j = 0; j < polygonCount; ++j) {
            //ワールド行列乗算済みポリゴンを取得する
            const auto& polygon = mesh.getPolygon(i, j, world);

            Vector3 interPoint{};
            //ポリゴンとレイが衝突していないなら次へ
            if (!Intersect::intersectRayPolygon(ray, polygon, &interPoint)) {
                continue;
            }

            if (raycastHit) {
                //衝突情報を記録する
                float dist = (interPoint - ray.start).lengthSq();
                IntersectHelper::updateRaycastHit(*raycastHit, &transform.gameObject(), dist, interPoint, polygon);
            } else {
                //衝突情報を記録する必要がなければ即終了
                return true;
            }
        }
    }

    return (raycastHit) ? raycastHit->isHit : false;
}

bool Intersect::intersectRayMeshes(const Ray& ray, const IMeshesGetter& meshesGetter, RaycastHit* raycastHit) {
    const auto& meshes = meshesGetter.getMeshes();
    for (const auto& mesh : meshes) {
        if (RaycastHit hit{}; intersectRayMesh(
            ray,
            *mesh->getMeshComponent().getMesh(),
            mesh->transform(),
            &hit
        )) {
            if (raycastHit) {
                //衝突情報を記録する
                float dist = (hit.point - ray.start).lengthSq();
                IntersectHelper::updateRaycastHit(*raycastHit, hit);
            } else {
                //衝突情報を記録する必要がなければ即終了
                return true;
            }
        }
    }

    return (raycastHit) ? raycastHit->isHit : false;
}
