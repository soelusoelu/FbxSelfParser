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
    std::vector<Material>& materials,
    std::vector<Motion>& motions,
    std::vector<Bone>& bones
) {
    //解析開始
    FbxParser parser;
    parser.parse(filePath);

    //現状1つ
    meshesVertices.resize(1);
    meshesVerticesPosition.resize(1);
    meshesIndices.resize(1);
    materials.resize(1);

    auto& meshVertices = meshesVertices[0];
    parser.getMeshParser().parse(meshVertices, meshesIndices[0]);

    auto& meshVerticesPosition = meshesVerticesPosition[0];
    auto size = meshVertices.size();
    meshVerticesPosition.resize(size);
    for (size_t i = 0; i < size; ++i) {
        meshVerticesPosition[i] = meshVertices[i].pos;
    }

    parser.getMaterialParser().parse(materials[0], filePath);
    parser.getBoneParser().parse(bones, meshVertices, meshesIndices[0], parser.getMeshParser());
    parser.getAnimationParser().parse(motions, bones);
}
