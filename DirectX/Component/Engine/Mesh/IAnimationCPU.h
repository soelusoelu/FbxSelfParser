#pragma once

#include "../../../Math/Math.h"
#include "../../../Mesh/IMeshLoader.h"
#include <vector>

//アニメーションCPU用インターフェース
class IAnimationCPU {
public:
    virtual ~IAnimationCPU() = default;
    //モーション後の頂点位置の配列を取得する
    virtual const MeshVerticesPosition& getCurrentMotionVertexPositions(unsigned index) const = 0;
};
