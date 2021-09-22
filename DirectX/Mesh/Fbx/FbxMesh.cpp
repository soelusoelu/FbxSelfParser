#include "FbxMesh.h"
#include <cassert>
#include <sstream>
#include <string>

FbxMesh::FbxMesh(const FbxObject& objectsObject)
    : mObjectsObject(objectsObject)
    , mLclTranslation(Vector3::zero)
    , mLclRotation(Quaternion::identity)
    , mLclScaling(Vector3::one)
    , mLclMatrix(Matrix4::identity)
{
    const auto& geometryObject = mObjectsObject.getObject("Geometry");

    parseLclMatrix();
    parseVertices(geometryObject);
    parseIndices(geometryObject);
    parseNormals(geometryObject);
    parseUV(geometryObject);
}

FbxMesh::~FbxMesh() = default;

void FbxMesh::parse(MeshVertices& meshVertices, Indices& indices) const {
    auto size = mNormals.size();
    meshVertices.resize(size);
    indices.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& v = meshVertices[i];
        indices[i] = static_cast<unsigned short>(i);
        v.pos = mVertices[mIndices[i]];
        v.normal = mNormals[i];
        v.uv = mUVs[mUVIndices[i]];
    }
}

void FbxMesh::parseLclMatrix() {
    const auto& lclMesh = mObjectsObject.getObject("Model", "Mesh");

    if (lclMesh.hasProperties("Lcl Scaling")) {
        const auto& value = lclMesh.getProperties("Lcl Scaling").value;
        std::istringstream iss(value);

        iss >> mLclScaling.x >> mLclScaling.y >> mLclScaling.z;
        mLclMatrix = Matrix4::createScale(mLclScaling);
    }
    if (lclMesh.hasProperties("Lcl Rotation")) {
        const auto& value = lclMesh.getProperties("Lcl Rotation").value;
        std::istringstream iss(value);

        Vector3 euler;
        iss >> euler.x >> euler.y >> euler.z;
        mLclRotation.setEuler(euler);

        mLclMatrix *= Matrix4::createFromQuaternion(mLclRotation);
    }
    if (lclMesh.hasProperties("Lcl Translation")) {
        const auto& value = lclMesh.getProperties("Lcl Translation").value;
        std::istringstream iss(value);

        iss >> mLclTranslation.x >> mLclTranslation.y >> mLclTranslation.z;
        mLclMatrix *= Matrix4::createTranslation(mLclTranslation);
    }
}

void FbxMesh::parseVertices(const FbxObject& geometryObject) {
    const auto& vertices = geometryObject.getArray("Vertices");
    auto size = vertices.size() / 3;
    mVertices.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& v = mVertices[i];
        auto idx = i * 3;
        v.x = std::stof(vertices[idx]);
        v.y = std::stof(vertices[idx + 1]);
        v.z = std::stof(vertices[idx + 2]);

        v = Vector3::transform(v, mLclMatrix);
    }
}

void FbxMesh::parseIndices(const FbxObject& geometryObject) {
    const auto& indices = geometryObject.getArray("PolygonVertexIndex");
    auto size = indices.size();
    mIndices.resize(size);

    for (size_t i = 0; i < size; ++i) {
        int tmp = std::stoi(indices[i]);

        //何故かマイナスのインデックスがあるから調整する
        if (tmp < 0) {
            tmp = Math::abs(tmp) - 1;
        }

        mIndices[i] = static_cast<unsigned short>(tmp);
    }
}

void FbxMesh::parseNormals(const FbxObject& geometryObject) {
    const auto& normalObject = geometryObject.getObject("LayerElementNormal");
    const auto& normals = normalObject.getArray("Normals");
    auto size = normals.size() / 3;
    mNormals.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& n = mNormals[i];
        auto idx = i * 3;
        n.x = std::stof(normals[idx]);
        n.y = std::stof(normals[idx + 1]);
        n.z = std::stof(normals[idx + 2]);

        n = Vector3::transform(n, mLclRotation);
    }
}

void FbxMesh::parseUV(const FbxObject& geometryObject) {
    const auto& uvObject = geometryObject.getObject("LayerElementUV");
    const auto& uvs = uvObject.getArray("UV");
    auto size = uvs.size() / 2;
    mUVs.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& uv = mUVs[i];
        auto idx = i * 2;
        uv.x = std::stof(uvs[idx]);
        uv.y = 1.f - std::stof(uvs[idx + 1]);
    }

    const auto& uvIndices = uvObject.getArray("UVIndex");
    size = uvIndices.size();
    mUVIndices.resize(size);

    for (size_t i = 0; i < size; ++i) {
        mUVIndices[i] = static_cast<unsigned short>(std::stoi(uvIndices[i]));
    }
}
