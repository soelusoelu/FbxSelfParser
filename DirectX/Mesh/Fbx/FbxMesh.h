#pragma once

#include "Reader/FbxObject.h"
#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include <unordered_map>
#include <vector>

class FbxMesh {
    struct LclMatrix {
        Vector3 lclTranslation = Vector3::zero;
        Quaternion lclRotation = Quaternion::identity;
        Vector3 lclScaling = Vector3::one;
        Matrix4 lclMatrix = Matrix4::identity;
    };

public:
    FbxMesh(
        const FbxObject& objectsObject,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxMesh();
    FbxMesh(const FbxMesh&) = delete;
    FbxMesh& operator=(const FbxMesh&) = delete;

    void parse(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices
    ) const;
    const std::vector<unsigned short>& getIndices(unsigned index) const;
    const std::unordered_map<unsigned, unsigned>& getLclModelNodeIDs() const;

private:
    void parseLclMatrix(
        const FbxObject& lclModelObject
    );
    //頂点を読み込む
    void parseVertices(
        std::vector<Vector3>& out,
        const FbxObject& geometryObject,
        const Matrix4& lclMatrix
    );
    //インデックスを読み込む
    void parseIndices(
        std::vector<unsigned short>& out,
        const FbxObject& geometryObject
    );
    //法線を読み込む
    void parseNormals(
        std::vector<Vector3>& out,
        const FbxObject& geometryObject,
        const Quaternion& lclRotation
    );
    //UVを読み込む
    void parseUV(
        std::vector<Vector2>& outUV,
        std::vector<unsigned short>& outUVIndices,
        const FbxObject& geometryObject
    );

private:
    const FbxObject& mObjectsObject;
    const std::unordered_multimap<unsigned, unsigned>& mConnections;

    //頂点配列
    std::vector<std::vector<Vector3>> mVertices;
    //インデックス配列
    std::vector<std::vector<unsigned short>> mIndices;
    //法線配列
    std::vector<std::vector<Vector3>> mNormals;
    //UV配列
    std::vector<std::vector<Vector2>> mUVs;
    //UVインデックス配列
    std::vector<std::vector<unsigned short>> mUVIndices;

    //lcl行列とノードIDのマップ
    std::unordered_map<unsigned, LclMatrix> mLclMatrixConnections;
    //key: lclModelオブジェクトのノードID, value: 添字
    std::unordered_map<unsigned, unsigned> mLclModelNodeIDMap;
};
