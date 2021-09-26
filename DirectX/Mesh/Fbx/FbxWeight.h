#pragma once

#include "Reader/FbxObject.h"
#include "../IMeshLoader.h"

class FbxMesh;

class FbxWeight {
public:
    FbxWeight(const FbxObject& objectsObject);
    ~FbxWeight();
    FbxWeight(const FbxWeight&) = delete;
    FbxWeight& operator=(const FbxWeight&) = delete;

    void parse(MeshVertices& meshVertices, const Indices& indices, const FbxMesh& mesh);

private:
    void parseWeight(MeshVertices& meshVertices, const Indices& indices, const FbxMesh& mesh, const FbxObject& deformerObject, unsigned boneIndex);

    //新しいウェイトを格納する
    void addWeight(MeshVertex& vertex, float weight, int boneIndex);
    //頂点ウェイトを正規化する
    void normalizeWeight(MeshVertices& meshVertice);

private:
    const FbxObject& mObjectsObject;

    static constexpr int MAX_INFLUENCE = 4;
};
