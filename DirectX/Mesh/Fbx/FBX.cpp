#include "FBX.h"
#include "FbxMaterial.h"
#include "FbxMesh.h"
#include "FbxParser.h"
#include "../../System/AssetsManager.h"
#include "../../Utility/FileUtil.h"
#include <fstream>
#include <sstream>

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

    const auto& mesh = parser.getMesh();
    const auto& vertices = mesh.getVertices();
    const auto& indices = mesh.getIndices();
    const auto& normals = mesh.getNormals();
    const auto& uvs = mesh.getUVs();
    auto size = vertices.size();

    auto& meshVertices = meshesVertices[0];
    auto& meshVerticesPosition = meshesVerticesPosition[0];
    auto& meshIndices = meshesIndices[0];
    meshVertices.resize(size);
    meshVerticesPosition.resize(size);
    meshIndices.resize(size);
    for (size_t i = 0; i < size; ++i) {
        auto& v = meshVertices[i];
        v.pos = vertices[i];
        v.normal = normals[i];
        v.uv = uvs[i];

        meshVerticesPosition[i] = vertices[i];

        meshIndices[i] = indices[i];
    }

    const auto& material = parser.getMaterial();
    const auto& mat = material.getMaterial();
    auto& outMat = materials[0];
    memcpy(&outMat, &mat, sizeof(Material));

    const auto& baseTexture = material.getBaseTextureName();
    if (baseTexture.size() > 0) {
        auto directryPath = FileUtil::getDirectryFromFilePath(filePath);
        outMat.textureID = AssetsManager::instance().createTexture(baseTexture, directryPath);
    }
}
