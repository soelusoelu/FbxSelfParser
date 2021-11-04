#include "FBX.h"
#include "FbxAnimation.h"
#include "FbxBone.h"
#include "FbxMaterial.h"
#include "FbxMesh.h"
#include "FbxParser.h"
#include "../../System/AssetsManager.h"
#include "../../Utility/FileUtil.h"

FBX::FBX() = default;

FBX::~FBX() = default;

void FBX::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<MeshVerticesPosition>& meshesVerticesPosition,
    std::vector<Indices>& meshesIndices,
    std::vector<int>& materialIDs,
    std::vector<Motion>& motions,
    std::vector<Bone>& bones
) {
    //解析開始
    FbxParser parser;
    parser.parse(filePath, meshesVertices, meshesIndices, materialIDs, bones, motions);

    auto meshCount = meshesVertices.size();
    meshesVerticesPosition.resize(meshCount);
    for (size_t i = 0; i < meshCount; ++i) {
        auto& meshVerticesPosition = meshesVerticesPosition[i];
        const auto& meshVertices = meshesVertices[i];

        auto size = meshVertices.size();
        meshVerticesPosition.resize(size);

        for (size_t j = 0; j < size; ++j) {
            meshVerticesPosition[j] = meshVertices[j].pos;
        }
    }
}
