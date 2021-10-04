#pragma once

#include "../../IMeshLoader.h"
#include <unordered_map>

class FbxMesh;
class FbxBone;

class FbxToDirectXMeshConverter {
public:
    FbxToDirectXMeshConverter(
        const FbxMesh& meshParser,
        const FbxBone& boneParser,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxToDirectXMeshConverter();
    FbxToDirectXMeshConverter(const FbxToDirectXMeshConverter&) = delete;
    FbxToDirectXMeshConverter& operator=(const FbxToDirectXMeshConverter&) = delete;

    //頂点バッファ、インデックスバッファをDirectXで求められる形に変換する
    void convert(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices
    ) const;

private:
    //頂点に位置、法線、UVを格納する
    void positionNormalUV(
        MeshVertices& meshVertices,
        Indices& meshIndices,
        unsigned meshIndex
    ) const;

    //頂点に位置、法線、UV + ウェイトを格納する
    void positionNormalUVAndWeight(
        MeshVertices& meshVertices,
        Indices& meshIndices,
        unsigned meshIndex
    ) const;

    //新しいウェイトを格納する
    void addWeight(
        MeshVertex& vertex,
        float weight,
        int boneIndex
    ) const;

    //頂点ウェイトを正規化する
    void normalizeWeight(
        MeshVertices& meshVertice
    ) const;

private:
    const FbxMesh& mMeshParser;
    const FbxBone& mBoneParser;
    const std::unordered_multimap<unsigned, unsigned>& mConnections;
};
