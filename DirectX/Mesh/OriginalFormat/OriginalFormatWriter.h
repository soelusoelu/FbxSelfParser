#pragma once

#include "../Material.h"
#include "../../Math/Math.h"
#include "../../System/Json/JsonObject.h"
#include <string>
#include <vector>

class FbxParser;
class FbxMesh;
class FbxMaterial;

class OriginalFormatWriter {
public:
    OriginalFormatWriter();
    ~OriginalFormatWriter();
    OriginalFormatWriter(const OriginalFormatWriter&) = delete;
    OriginalFormatWriter& operator=(const OriginalFormatWriter&) = delete;

    void writeFbxToOriginal(const std::string& filePath, const FbxParser& fbx) const;

private:
    void writeMeshes(JsonObject& out, const FbxMesh& mesh, const FbxMaterial& material) const;
    void writeMesh(
        JsonObject& out,
        const std::vector<Vector3>& vertices,
        const std::vector<unsigned short>& indices,
        const std::vector<Vector3>& normals,
        const std::vector<Vector2>& uvs,
        const std::vector<unsigned short>& uvIndices
    ) const;

    void writeMaterials(const std::string& filePath, const FbxMaterial& material) const;
    void writeMaterial(JsonObject& out, const Material& material) const;
};
