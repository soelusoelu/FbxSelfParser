#include "VertexSelector.h"
#include "../../Collision/Collision.h"
#include "../../Engine/Camera/SimpleCamera.h"
#include "../../Input/Input.h"

bool VertexSelector::selectVertex(
    Vector3& selected,
    const std::vector<Vector3>& vertices,
    const SimpleCamera& camera,
    float vertexRadius
) {
    //カメラからマウス位置に向かうレイを取得する
    const auto& mousePos = Input::mouse().getMousePosition();
    auto ray = camera.screenToRay(mousePos);

    //当たり判定を取るための球
    Sphere s{};
    s.radius = vertexRadius;

    //準備
    float min = FLT_MAX;
    const auto& camPos = camera.getPosition();
    bool isSelect = false;

    //全頂点から最近点を選択する
    for (const auto& v : vertices) {
        s.center = v;

        if (Intersect::intersectRaySphere(ray, s)) {
            auto length = (v - camPos).lengthSq();
            if (length < min) {
                min = length;
                selected = v;
            }

            isSelect = true;
        }
    }

    return isSelect;
}

bool VertexSelector::selectVertexFromModel(
    Vector3& selected,
    const IMesh* mesh,
    const SimpleCamera& camera,
    float vertexRadius
) {
    //準備
    auto meshCount = mesh->getMeshCount();
    const auto& camPos = camera.getPosition();
    float min = FLT_MAX;
    bool isSelect = false;

    //最近点の頂点を選択する
    for (unsigned i = 0; i < meshCount; ++i) {
        Vector3 out;
        if (!selectVertex(out, mesh->getMeshVerticesPosition(i), camera, vertexRadius)) {
            continue;
        }

        auto length = (out - camPos).lengthSq();
        if (length < min) {
            min = length;
            selected = out;
        }

        isSelect = true;
    }

    return isSelect;
}
