﻿#pragma once

#include "Bone.h"
#include "Material.h"
#include "Motion.h"
#include "Vertex.h"
#include <memory>
#include <string>
#include <vector>

//頂点構造体の配列を別名定義
using MeshVertices = std::vector<MeshVertex>;
//頂点位置の配列を別名定義
using MeshVerticesPosition = std::vector<Vector3>;
//インデックスの配列を別名定義
using Indices = std::vector<unsigned short>;

class IMeshLoader {
public:
    virtual ~IMeshLoader() = default;
    virtual void parse(
        const std::string& filePath,
        std::vector<MeshVertices>& meshesVertices,
        std::vector<MeshVerticesPosition>& meshesVerticesPosition,
        std::vector<Indices>& meshesIndices,
        std::vector<int>& materialIDs,
        std::vector<Motion>& motions,
        std::vector<Bone>& bones
    ) = 0;
};
