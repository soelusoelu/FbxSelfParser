#pragma once

#include "../IMeshLoader.h"
#include "../VertexType.h"
#include "../../System/Json/JsonObject.h"
#include <string>

class OriginalFormatToDirectXConverter {
public:
    OriginalFormatToDirectXConverter();
    ~OriginalFormatToDirectXConverter();
    OriginalFormatToDirectXConverter(const OriginalFormatToDirectXConverter&) = delete;
    OriginalFormatToDirectXConverter& operator=(const OriginalFormatToDirectXConverter&) = delete;

    //DirectXで求められる形に変換する
    void convert(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<MeshVerticesPosition>& meshesVerticesPosition,
        std::vector<Indices>& meshesIndices,
        std::vector<int>& materialIDs,
        std::vector<Bone>& bones,
        std::vector<Motion>& motions,
        const std::string& filePath,
        const JsonObject& rootObj
    ) const;

private:
    void convertMeshes(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<MeshVerticesPosition>& meshesVerticesPosition,
        std::vector<Indices>& meshesIndices,
        std::vector<int>& materialIDs,
        const std::string& directoryPath,
        const JsonObject& rootObj
    ) const;
    void convertMesh(
        MeshVertices& meshVertices,
        MeshVerticesPosition& meshVerticesPosition,
        Indices& meshIndices,
        int& materialID,
        const std::string& directoryPath,
        const JsonObject& meshObj,
        VertexType vertexType
    ) const;
    void convertWeights(
        MeshVertices& meshVertices,
        const JsonObject& meshObj
    ) const;

    void convertMaterial(
        int& materialID,
        const std::string& directoryPath,
        const std::string& materialName
    ) const;

    void convertBones(
        std::vector<Bone>& bones,
        const JsonObject& rootObj
    ) const;
    void convertBone(
        Bone& bone,
        std::vector<Bone>& bones,
        const JsonObject& boneObj
    ) const;

    void convertAnimations(
        std::vector<Motion>& motions,
        const JsonObject& rootObj
    ) const;
    void convertAnimation(
        Motion& motion,
        const JsonObject& animationObj
    ) const;
};
