#pragma once

#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include "../../System/Json/JsonObject.h"
#include "../../System/Json/JsonValue.h"
#include <string>

class FbxParser;

class OriginalFormatWriter {
public:
    OriginalFormatWriter();
    ~OriginalFormatWriter();
    OriginalFormatWriter(const OriginalFormatWriter&) = delete;
    OriginalFormatWriter& operator=(const OriginalFormatWriter&) = delete;

    void writeFbxToOriginal(
        const std::string& filePath,
        const std::vector<MeshVertices>& meshesVertices,
        const std::vector<Indices>& meshesIndices,
        const std::vector<int>& materialIDs,
        const std::vector<Bone>& bones,
        const std::vector<Motion>& motions
    ) const;

private:
    void writeMeshes(
        JsonObject& out,
        const std::vector<MeshVertices>& meshesVertices,
        const std::vector<Indices>& meshesIndices,
        const std::vector<int>& materialIDs,
        bool hasBone
    ) const;
    void writeMesh(
        JsonObject& out,
        const MeshVertices& meshVertices,
        const Indices& meshIndices,
        bool hasBone
    ) const;
    void writeWeights(
        JsonObject& out,
        const MeshVertices& meshVertices
    ) const;

    void writeBones(
        JsonObject& out,
        const std::vector<Bone>& bones
    ) const;
    void writeBone(
        std::vector<JsonValue>& out,
        const Bone& bone
    ) const;

    void writeAnimations(
        JsonObject& out,
        const std::vector<Motion>& motions
    ) const;
    void writeAnimations(
        JsonObject& out,
        const Motion& motion
    ) const;

    void writeMaterials(
        const std::string& filePath,
        const std::vector<int>& materialIDs
    ) const;
    void writeMaterial(
        JsonObject& out,
        const Material& material
    ) const;
};
