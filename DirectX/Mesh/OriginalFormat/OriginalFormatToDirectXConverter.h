#pragma once

#include "../IMeshLoader.h"
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
        const JsonObject& rootObj
    ) const;

private:
    void convertMeshes(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<MeshVerticesPosition>& meshesVerticesPosition,
        std::vector<Indices>& meshesIndices,
        std::vector<int>& materialIDs,
        const JsonObject& rootObj
    ) const;
    void convertMesh(
        MeshVertices& meshVertices,
        MeshVerticesPosition& meshVerticesPosition,
        Indices& meshIndices,
        int& materialID,
        const JsonObject& meshObj
    ) const;

    void convertMaterial(
        int& materialID,
        const std::string& materialName
    ) const;
};
