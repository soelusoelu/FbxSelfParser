#pragma once

#include "../IMeshLoader.h"
#include "../../System/Json/JsonObject.h"

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
        std::vector<Material>& materials,
        const JsonObject& rootObj
    ) const;

private:
    void convertMeshes(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<MeshVerticesPosition>& meshesVerticesPosition,
        std::vector<Indices>& meshesIndices,
        const JsonObject& rootObj
    ) const;
    void convertMesh(
        MeshVertices& meshVertices,
        MeshVerticesPosition& meshVerticesPosition,
        Indices& meshIndices,
        const JsonObject& meshObj
    ) const;

    void convertMaterials(
        std::vector<Material>& materials,
        const JsonObject& rootObj
    ) const;
    void convertMaterial(
        Material& material,
        const JsonObject& matObj
    ) const;
};
