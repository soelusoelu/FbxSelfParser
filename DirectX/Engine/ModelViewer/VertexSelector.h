#pragma once

#include "../../Math/Math.h"
#include "../../Mesh/IMesh.h"
#include <vector>

class SimpleCamera;

class VertexSelector {
private:
    VertexSelector() = delete;
    ~VertexSelector() = delete;

public:
    //全頂点から最近点を選択する
    static bool selectVertex(
        Vector3& selected,
        const std::vector<Vector3>& vertices,
        const SimpleCamera& camera,
        float vertexRadius
    );

    //モデルの全頂点から最近点を選択する
    static bool selectVertexFromModel(
        Vector3& selected,
        const IMesh* mesh,
        const SimpleCamera& camera,
        float vertexRadius
    );

private:
    VertexSelector(const VertexSelector&) = delete;
    VertexSelector& operator=(const VertexSelector&) = delete;
};
