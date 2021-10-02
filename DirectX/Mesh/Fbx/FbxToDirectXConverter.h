#pragma once

#include "../IMeshLoader.h"
#include "../Bone.h"
#include <unordered_map>
#include <vector>

class FbxMesh;
class FbxBone;

class FbxToDirectXConverter {
public:
    FbxToDirectXConverter(
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxToDirectXConverter();
    FbxToDirectXConverter(const FbxToDirectXConverter&) = delete;
    FbxToDirectXConverter& operator=(const FbxToDirectXConverter&) = delete;

    void convertVerticesAndIndices(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices,
        const FbxMesh& meshParser,
        const FbxBone& boneParser
    ) const;

    void convertBone(
        std::vector<Bone>& bones,
        const FbxBone& boneParser
    ) const;

private:
    //頂点
    void positionNormalUV(
        MeshVertices& meshVertices,
        Indices& meshIndices,
        const FbxMesh& meshParser,
        unsigned meshIndex
    ) const;
    void positionNormalUVAndWeight(
        MeshVertices& meshVertices,
        Indices& meshIndices,
        const FbxMesh& meshParser,
        const FbxBone& boneParser,
        unsigned meshIndex
    ) const;
    //新しいウェイトを格納する
    void addWeight(MeshVertex& vertex, float weight, int boneIndex) const;
    //頂点ウェイトを正規化する
    void normalizeWeight(MeshVertices& meshVertice) const;

private:
    const std::unordered_multimap<unsigned, unsigned>& mConnections;

    static constexpr int MAX_INFLUENCE = 4;
};
