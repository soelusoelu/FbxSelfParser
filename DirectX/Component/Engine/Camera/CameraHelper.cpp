#include "CameraHelper.h"
#include "../../../Collision/Collision.h"
#include "../../../System/Window.h"

Vector3 CameraHelper::screenToWorldPoint(
    const Vector2& position,
    const Matrix4& view,
    const Matrix4& projection,
    float z
) {
    //ビューポートの逆行列を求める
    auto viewport = Matrix4::identity;
    viewport.m[0][0] = Window::width() / 2.f;
    viewport.m[1][1] = -Window::height() / 2.f;
    viewport.m[3][0] = Window::width() / 2.f;
    viewport.m[3][1] = Window::height() / 2.f;

    //ビュー * 射影 * ビューポートの逆行列を求める
    auto invMat = Matrix4::inverse(view * projection * viewport);

    //スクリーン座標をワールド座標に変換
    return Vector3::transformWithPerspDiv(Vector3(position, z), invMat);
}

Ray CameraHelper::screenToRay(
    const Vector3& start,
    const Vector2& end,
    const Matrix4& view,
    const Matrix4& projection,
    float z
) {
    Ray ray{};
    ray.start = start;
    ray.end = screenToWorldPoint(end, view, projection, z);

    return ray;
}

Matrix4 CameraHelper::getViewMatrixTakingSphereInCamera(
    const Sphere& sphere,
    int width,
    int height,
    float fov,
    const Vector3& direction,
    const Vector3& up
) {
    //カメラ位置を決定する
    auto pos = getCameraPositionTakingSphereInCamera(sphere, width, height, fov, direction);

    // ビュー行列作成
    return Matrix4::createLookAt(pos, sphere.center, up);
}

Vector3 CameraHelper::getCameraPositionTakingSphereInCamera(
    const Sphere& sphere,
    int width,
    int height,
    float fov,
    const Vector3& direction
) {
    float aspectYDivX = static_cast<float>(height) / static_cast<float>(width);

    //fovYとfovXの小さい方をθとして選択
    float theta = (aspectYDivX >= 1.f) ? fov * aspectYDivX : fov;

    //カメラを引く距離を算出
    float d = sphere.radius / Math::sin(theta / 2.f);

    //カメラ位置を決定する
    return sphere.center - direction * d;
}
