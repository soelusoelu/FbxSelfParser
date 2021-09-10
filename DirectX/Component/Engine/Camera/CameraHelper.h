#pragma once

#include "../../../Math/Math.h"

struct Sphere;
struct Ray;

class CameraHelper {
private:
    CameraHelper() = delete;
    ~CameraHelper() = delete;

public:
    //スクリーン座標をワールド座標に変換する
    //zが0のときカメラから最も近い点、1のとき最も遠い点を計算する z[0, 1]
    static Vector3 screenToWorldPoint(
        const Vector2& position,
        const Matrix4& view,
        const Matrix4& projection,
        float z = 1.f
    );

    //カメラ位置からスクリーン座標からワールド座標に変換した点へのレイを取得する
    //zが0のときカメラから最も近い点、1のとき最も遠い点を計算する z[0, 1]
    static Ray screenToRay(
        const Vector3& start,
        const Vector2& end,
        const Matrix4& view,
        const Matrix4& projection,
        float z = 1.f
    );

    //スフィアがカメラに収まるビュー行列を作成する
    static Matrix4 getViewMatrixTakingSphereInCamera(
        const Sphere& sphere,
        int width,
        int height,
        float fov,
        const Vector3& direction,
        const Vector3& up
    );

    //スフィアが収まるカメラ位置を取得する
    static Vector3 getCameraPositionTakingSphereInCamera(
        const Sphere& sphere,
        int width,
        int height,
        float fov,
        const Vector3& direction
    );
};
