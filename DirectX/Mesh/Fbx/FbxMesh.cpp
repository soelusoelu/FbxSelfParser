#include "FbxMesh.h"
#include <cassert>
#include <string>

FbxMesh::FbxMesh(const FbxObject& geometryObject)
    : mGeometryObject(geometryObject)
{
    parse();
}

FbxMesh::~FbxMesh() = default;

const std::vector<Vector3>& FbxMesh::getVertices() const {
    return mSurfaceVertices;
}

const std::vector<unsigned short>& FbxMesh::getIndices() const {
    return mSurfaceIndices;
}

const std::vector<Vector3>& FbxMesh::getNormals() const {
    return mSurfaceNormals;
}

const std::vector<Vector2>& FbxMesh::getUVs() const {
    return mSurfaceUVs;
}

void FbxMesh::parse() {
    parseVertices();
    parseIndices();
    parseNormals();
    parseUV();

    auto size = mSurfaceNormals.size();
    mSurfaceVertices.resize(size);
    mSurfaceIndices.resize(size);
    mSurfaceUVs.resize(size);

    for (size_t i = 0; i < size; ++i) {
        mSurfaceIndices[i] = i;
        mSurfaceVertices[i] = mVertices[mIndices[i]];
        mSurfaceUVs[i] = mUVs[mUVIndices[i]];
    }
}

void FbxMesh::parseVertices() {
    const auto& vertices = mGeometryObject.getArray("Vertices");
    auto size = vertices.size() / 3;
    mVertices.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& v = mVertices[i];
        auto idx = i * 3;
        v.x = std::stof(vertices[idx]);
        v.y = std::stof(vertices[idx + 1]);
        v.z = std::stof(vertices[idx + 2]);
    }
}

void FbxMesh::parseIndices() {
    const auto& indices = mGeometryObject.getArray("PolygonVertexIndex");
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

void FbxMesh::parseNormals() {
    const auto& normalObject = mGeometryObject.getObject("LayerElementNormal");
    const auto& normals = normalObject.getArray("Normals");
    auto size = normals.size() / 3;
    mSurfaceNormals.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& n = mSurfaceNormals[i];
        auto idx = i * 3;
        n.x = std::stof(normals[idx]);
        n.y = std::stof(normals[idx + 1]);
        n.z = std::stof(normals[idx + 2]);
    }
}

void FbxMesh::parseUV() {
    const auto& uvObject = mGeometryObject.getObject("LayerElementUV");
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
