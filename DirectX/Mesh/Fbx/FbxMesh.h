#pragma once

#include "Reader/FbxObject.h"
#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include <vector>

class FbxMesh {
public:
    FbxMesh(const FbxObject& objectsObject);
    ~FbxMesh();
    FbxMesh(const FbxMesh&) = delete;
    FbxMesh& operator=(const FbxMesh&) = delete;

    void parse(MeshVertices& meshVertices, Indices& indices) const;
    const std::vector<unsigned short>& getIndices() const;

private:
    void parseLclMatrix();
    //頂点を読み込む
    void parseVertices(const FbxObject& geometryObject);
    //インデックスを読み込む
    void parseIndices(const FbxObject& geometryObject);
    //法線を読み込む
    void parseNormals(const FbxObject& geometryObject);
    //UVを読み込む
    void parseUV(const FbxObject& geometryObject);

private:
    const FbxObject& mObjectsObject;

    //頂点配列
    std::vector<Vector3> mVertices;
    //インデックス配列
    std::vector<unsigned short> mIndices;
    //法線配列
    std::vector<Vector3> mNormals;
    //UV配列
    std::vector<Vector2> mUVs;
    //UVインデックス配列
    std::vector<unsigned short> mUVIndices;

    Vector3 mLclTranslation;
    Quaternion mLclRotation;
    Vector3 mLclScaling;
    Matrix4 mLclMatrix;
};
