#include "OriginalFormatToDirectXConverter.h"
#include "../../System/Json/JsonValue.h"

OriginalFormatToDirectXConverter::OriginalFormatToDirectXConverter() = default;

OriginalFormatToDirectXConverter::~OriginalFormatToDirectXConverter() = default;

void OriginalFormatToDirectXConverter::convert(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<MeshVerticesPosition>& meshesVerticesPosition,
    std::vector<Indices>& meshesIndices,
    std::vector<int>& materialIDs,
    const JsonObject& rootObj
) const {
    convertMeshes(meshesVertices, meshesVerticesPosition, meshesIndices, rootObj);
    convertMaterials(materialIDs, rootObj);
}

void OriginalFormatToDirectXConverter::convertMeshes(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<MeshVerticesPosition>& meshesVerticesPosition,
    std::vector<Indices>& meshesIndices,
    const JsonObject& rootObj
) const {
    const auto& meshVal = rootObj.getValue("mesh");
    const auto& meshObj = meshVal.getObject();
    int meshCount = meshObj.getValue("meshCount").getInt();
    meshesVertices.resize(meshCount);
    meshesVerticesPosition.resize(meshCount);
    meshesIndices.resize(meshCount);

    const auto& meshesVal = meshObj.getValue("meshes");
    const auto& meshesArray = meshesVal.getArray();
    for (int i = 0; i < meshCount; ++i) {
        convertMesh(meshesVertices[i], meshesVerticesPosition[i], meshesIndices[i], meshesArray[i].getObject());
    }
}

void OriginalFormatToDirectXConverter::convertMesh(
    MeshVertices& meshVertices,
    MeshVerticesPosition& meshVerticesPosition,
    Indices& meshIndices,
    const JsonObject& meshObj
) const {
    int verticesCount = meshObj.getValue("verticesCount").getInt();
    meshVertices.resize(verticesCount);
    meshVerticesPosition.resize(verticesCount);
    meshIndices.resize(verticesCount);

    const auto& srcVertices = meshObj.getValue("vertices").getArray();
    const auto& srcIndices = meshObj.getValue("indices").getArray();
    const auto& srcNormals = meshObj.getValue("normals").getArray();
    const auto& srcUVs = meshObj.getValue("uvs").getArray();

    for (int i = 0; i < verticesCount; ++i) {
        //Vector2, 3用インデックス
        auto idx2 = i * 2;
        auto idx3 = i * 3;

        auto& v = meshVertices[i];
        v.pos.x = srcVertices[idx3].getFloat();
        v.pos.y = srcVertices[idx3 + 1].getFloat();
        v.pos.z = srcVertices[idx3 + 2].getFloat();

        meshVerticesPosition[i] = v.pos;

        meshIndices[i] = srcIndices[i].getInt();

        v.normal.x = srcNormals[idx3].getFloat();
        v.normal.y = srcNormals[idx3 + 1].getFloat();
        v.normal.z = srcNormals[idx3 + 2].getFloat();

        v.uv.x = srcUVs[idx2].getFloat();
        v.uv.x = srcUVs[idx2 + 1].getFloat();
    }
}

void OriginalFormatToDirectXConverter::convertMaterials(
    std::vector<int>& materialIDs,
    const JsonObject& rootObj
) const {

}

void OriginalFormatToDirectXConverter::convertMaterial(
    Material& material,
    const JsonObject& matObj
) const {

}
