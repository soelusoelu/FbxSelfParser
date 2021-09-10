#pragma once

#include "../Math/Math.h"
#include "../Mesh/IMesh.h"
#include "../Mesh/IMeshesGetter.h"

struct AABB;
struct OBB;
struct Circle;
struct Sphere;
struct Triangle;
struct Ray;
struct RaycastHit;
class Transform3D;

namespace Intersect {
//円同士の衝突判定を行う
bool intersectCircle(const Circle& a, const Circle& b);

//球同士の衝突判定を行う
bool intersectSphere(const Sphere& a, const Sphere& b);

//AABB同士の衝突判定を行う
bool intersectAABB(const AABB& a, const AABB& b);

//OBB同士の衝突判定を行う
bool intersectOBB(const OBB& a, const OBB& b);

//無限平面とレイの衝突判定を行う
bool intersectRayPlane(const Ray& ray, const Plane& p, Vector3* intersectPoint = nullptr);

//ポリゴンとレイの衝突判定を行う
bool intersectRayPolygon(const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector3* intersectPoint = nullptr);
bool intersectRayPolygon(const Ray& ray, const Triangle& polygon, Vector3* intersectPoint = nullptr);

//球とレイの衝突判定を行う
bool intersectRaySphere(const Ray& ray, const Sphere& sphere, Vector3* intersectPoint = nullptr);

//AABBとレイの衝突判定を行う
bool intersectRayAABB(const Ray& ray, const AABB& aabb);
bool intersectRayAABB(const Ray& ray, const AABB& aabb, Vector3& intersectPoint);

//OBBとレイの衝突判定を行う
bool intersectRayOBB(const Ray& ray, const OBB& obb);

//メッシュとレイの衝突判定を行う
//めちゃめちゃ重い
bool intersectRayMesh(const Ray& ray, const IMesh& mesh, const Transform3D& transform, RaycastHit* raycastHit = nullptr);
bool intersectRayMeshes(const Ray& ray, const IMeshesGetter& meshesGetter, RaycastHit* raycastHit = nullptr);
};
