#pragma once

#include "Reader/FbxObject.h"
#include "../../Math/Math.h"
#include <vector>

class FbxMesh {
public:
    FbxMesh(const FbxObject& objectsObject);
    ~FbxMesh();
    FbxMesh(const FbxMesh&) = delete;
    FbxMesh& operator=(const FbxMesh&) = delete;

    const std::vector<Vector3>& getVertices() const;
    const std::vector<unsigned short>& getIndices() const;
    const std::vector<Vector3>& getNormals() const;
    const std::vector<Vector2>& getUVs() const;

private:
    void parse();
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
    //面法線用頂点配列
    std::vector<Vector3> mSurfaceVertices;
    //インデックス配列
    std::vector<unsigned short> mIndices;
    //面法線用インデックス配列
    std::vector<unsigned short> mSurfaceIndices;
    //法線配列
    std::vector<Vector3> mSurfaceNormals;
    //UV配列
    std::vector<Vector2> mUVs;
    //面法線用UV配列
    std::vector<Vector2> mSurfaceUVs;
    //UVインデックス配列
    std::vector<unsigned short> mUVIndices;

    Vector3 mLclTranslation;
    Quaternion mLclRotation;
    Vector3 mLclScaling;
    Matrix4 mLclMatrix;
};
