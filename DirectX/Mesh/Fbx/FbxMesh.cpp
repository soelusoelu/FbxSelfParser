#include "FbxMesh.h"
#include <cassert>
#include <sstream>
#include <string>

FbxMesh::FbxMesh(const FbxObject& objectsObject, const std::multimap<unsigned, unsigned>& connections)
    : mObjectsObject(objectsObject)
    , mConnections(connections)
{
    const auto& children = mObjectsObject.children;
    //Lcl行列は先に取得しておく必要がある
    for (const auto& c : children) {
        if (c.name != "Model") {
            continue;
        }
        const auto& attributes = c.attributes;
        if (attributes[2] != "Mesh") {
            continue;
        }

        unsigned nodeID = static_cast<unsigned>(std::stoi(attributes[0]));
        parseLclMatrix(c, nodeID);
    }

    for (const auto& child : children) {
        if (child.name != "Geometry") {
            continue;
        }

        unsigned nodeID = static_cast<unsigned>(std::stoi(child.attributes[0]));
        auto range = mConnections.equal_range(nodeID);
        for (auto& r = range.first; r != range.second; ++r) {
            auto itr = mLclMatrixConnections.find(r->second);
            if (itr == mLclMatrixConnections.end()) {
                continue;
            }
            const auto& lclMatrix = itr->second;

            parseVertices(mVertices.emplace_back(), child, lclMatrix.lclMatrix);
            parseIndices(mIndices.emplace_back(), child);
            parseNormals(mNormals.emplace_back(), child, lclMatrix.lclRotation);
            parseUV(mUVs.emplace_back(), mUVIndices.emplace_back(), child);
        }
    }
}

FbxMesh::~FbxMesh() = default;

void FbxMesh::parse(std::vector<MeshVertices>& meshesVertices, std::vector<Indices>& meshesIndices) const {
    auto meshCount = mVertices.size();
    meshesVertices.resize(meshCount);
    meshesIndices.resize(meshCount);
    for (size_t i = 0; i < meshCount; ++i) {
        auto& meshVertices = meshesVertices[i];
        auto& meshIndices = meshesIndices[i];
        auto& vertices = mVertices[i];
        auto& indices = mIndices[i];
        const auto& normals = mNormals[i];
        const auto& uvs = mUVs[i];
        const auto& uvIndices = mUVIndices[i];

        auto size = mNormals[i].size();
        meshVertices.resize(size);
        meshIndices.resize(size);

        for (size_t j = 0; j < size; ++j) {
            auto& v = meshVertices[j];
            meshIndices[j] = static_cast<unsigned short>(j);
            v.pos = vertices[indices[j]];
            v.normal = normals[j];
            v.uv = uvs[uvIndices[j]];
        }
    }
}

const std::vector<unsigned short>& FbxMesh::getIndices() const {
    return mIndices[0];
}

const std::unordered_map<unsigned, unsigned>& FbxMesh::getLclModelNodeIDs() const {
    return mLclModelNodeIDMap;
}

void FbxMesh::parseLclMatrix(const FbxObject& modelObject, unsigned nodeID) {
    LclMatrix lclMatrix;

    if (modelObject.hasProperties("Lcl Scaling")) {
        const auto& value = modelObject.getProperties("Lcl Scaling").value;
        std::istringstream iss(value);

        auto& s = lclMatrix.lclScaling;
        iss >> s.x >> s.y >> s.z;
        lclMatrix.lclMatrix = Matrix4::createScale(s);
    }
    if (modelObject.hasProperties("Lcl Rotation")) {
        const auto& value = modelObject.getProperties("Lcl Rotation").value;
        std::istringstream iss(value);

        Vector3 euler;
        iss >> euler.x >> euler.y >> euler.z;
        lclMatrix.lclRotation.setEuler(euler);

        lclMatrix.lclMatrix *= Matrix4::createFromQuaternion(lclMatrix.lclRotation);
    }
    if (modelObject.hasProperties("Lcl Translation")) {
        const auto& value = modelObject.getProperties("Lcl Translation").value;
        std::istringstream iss(value);

        auto& t = lclMatrix.lclTranslation;
        iss >> t.x >> t.y >> t.z;
        lclMatrix.lclMatrix *= Matrix4::createTranslation(t);
    }

    mLclMatrixConnections.emplace(nodeID, lclMatrix);
    mLclModelNodeIDMap.emplace(nodeID, mLclModelNodeIDMap.size());
}

void FbxMesh::parseVertices(std::vector<Vector3>& out, const FbxObject& geometryObject, const Matrix4& lclMatrix) {
    const auto& vertices = geometryObject.getArray("Vertices");
    auto size = vertices.size() / 3;
    out.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& v = out[i];
        auto idx = i * 3;
        v.x = std::stof(vertices[idx]);
        v.y = std::stof(vertices[idx + 1]);
        v.z = std::stof(vertices[idx + 2]);

        v = Vector3::transform(v, lclMatrix);
    }
}

void FbxMesh::parseIndices(std::vector<unsigned short>& out, const FbxObject& geometryObject) {
    const auto& indices = geometryObject.getArray("PolygonVertexIndex");
    auto size = indices.size();
    out.resize(size);

    for (size_t i = 0; i < size; ++i) {
        int tmp = std::stoi(indices[i]);

        //多角形ポリゴンに対応するためにマイナスのインデックスがあるから調整する
        if (tmp < 0) {
            tmp = Math::abs(tmp) - 1;
        }

        out[i] = static_cast<unsigned short>(tmp);
    }
}

void FbxMesh::parseNormals(std::vector<Vector3>& out, const FbxObject& geometryObject, const Quaternion& lclRotation) {
    const auto& normalObject = geometryObject.getObject("LayerElementNormal");
    const auto& normals = normalObject.getArray("Normals");
    auto size = normals.size() / 3;
    out.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& n = out[i];
        auto idx = i * 3;
        n.x = std::stof(normals[idx]);
        n.y = std::stof(normals[idx + 1]);
        n.z = std::stof(normals[idx + 2]);

        n = Vector3::transform(n, lclRotation);
    }
}

void FbxMesh::parseUV(std::vector<Vector2>& outUV, std::vector<unsigned short>& outUVIndices, const FbxObject& geometryObject) {
    const auto& uvObject = geometryObject.getObject("LayerElementUV");
    const auto& uvs = uvObject.getArray("UV");
    auto size = uvs.size() / 2;
    outUV.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& uv = outUV[i];
        auto idx = i * 2;
        uv.x = std::stof(uvs[idx]);
        uv.y = 1.f - std::stof(uvs[idx + 1]);
    }

    const auto& uvIndices = uvObject.getArray("UVIndex");
    size = uvIndices.size();
    outUVIndices.resize(size);

    for (size_t i = 0; i < size; ++i) {
        outUVIndices[i] = static_cast<unsigned short>(std::stoi(uvIndices[i]));
    }
}
