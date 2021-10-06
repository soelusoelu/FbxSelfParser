#include "FbxMesh.h"
#include <cassert>
#include <sstream>
#include <string>

FbxMesh::FbxMesh(
    const FbxObject& globalSettingsObject,
    const FbxObject& objectsObject,
    const std::unordered_multimap<unsigned, unsigned>& connections
)
    : mCoordAxis(0)
    , mUpAxis(0)
    , mFrontAxis(0)
    , mCoordAxisSign(0)
    , mUpAxisSign(0)
    , mFrontAxisSign(0)
{
    //軸成分を取得する
    parseAxis(globalSettingsObject);

    //Lcl行列はメッシュより先に取得しておく必要がある
    parseLclMatrices(objectsObject);

    //メッシュの数はlclModelオブジェクトのノード数と一致する
    auto lclNodeCount = mLclModelNodeIDMap.size();
    mVertices.resize(lclNodeCount);
    mIndices.resize(lclNodeCount);
    mNormals.resize(lclNodeCount);
    mUVs.resize(lclNodeCount);
    mUVIndices.resize(lclNodeCount);

    //Geometryオブジェクトを解析する
    parseGeometry(objectsObject, connections);
}

FbxMesh::~FbxMesh() = default;

int FbxMesh::getMeshCount() const {
    return mVertices.size();
}

const std::vector<Vector3>& FbxMesh::getVertices(unsigned index) const {
    return mVertices[index];
}

const std::vector<unsigned short>& FbxMesh::getIndices(unsigned index) const {
    return mIndices[index];
}

const std::vector<Vector3>& FbxMesh::getNormals(unsigned index) const {
    return mNormals[index];
}

const std::vector<Vector2>& FbxMesh::getUVs(unsigned index) const {
    return mUVs[index];
}

const std::vector<unsigned short>& FbxMesh::getUVIndices(unsigned index) const {
    return mUVIndices[index];
}

const std::unordered_map<unsigned, unsigned short>& FbxMesh::geGeometryNodeIDs() const {
    return mGeometryNodeIDMap;
}

const std::unordered_map<unsigned, unsigned short>& FbxMesh::getLclModelNodeIDs() const {
    return mLclModelNodeIDMap;
}

void FbxMesh::parseAxis(
    const FbxObject& globalSettingsObject
) {
    const auto& coordAxis = globalSettingsObject.getProperties("CoordAxis").value;
    mCoordAxis = std::stoi(coordAxis);
    const auto& upAxis = globalSettingsObject.getProperties("UpAxis").value;
    mUpAxis = std::stoi(upAxis);
    const auto& frontAxis = globalSettingsObject.getProperties("FrontAxis").value;
    mFrontAxis = std::stoi(frontAxis);

    const auto& coordAxisSign = globalSettingsObject.getProperties("CoordAxisSign").value;
    mCoordAxisSign = std::stoi(coordAxisSign);
    const auto& upAxisSign = globalSettingsObject.getProperties("UpAxisSign").value;
    mUpAxisSign = std::stoi(upAxisSign);
    const auto& frontAxisSign = globalSettingsObject.getProperties("FrontAxisSign").value;
    mFrontAxisSign = std::stoi(frontAxisSign);
}

void FbxMesh::parseLclMatrices(
    const FbxObject& objectsObject
) {
    //Lcl行列は先に取得しておく必要がある
    const auto& children = objectsObject.children;
    //Modelに一致する子オブジェクトを取得する
    auto models = children.equal_range("Model");
    for (auto& m = models.first; m != models.second; ++m) {
        const auto& obj = m->second;
        const auto& attributes = obj.attributes;
        //アトリビュートの3つ目がMeshでなければ次へ
        if (attributes[2] != "Mesh") {
            continue;
        }

        //Lcl行列を解析する
        parseLclMatrix(obj);
    }
}

void FbxMesh::parseLclMatrix(
    const FbxObject& lclModelObject
) {
    LclMatrix lclMatrix;

    if (lclModelObject.hasProperties("Lcl Scaling")) {
        const auto& value = lclModelObject.getProperties("Lcl Scaling").value;
        std::istringstream iss(value);

        auto& s = lclMatrix.lclScaling;
        iss >> s.x >> s.y >> s.z;

        //float tmp[3] = { 0.f, 0.f, 0.f };
        //iss >> tmp[0] >> tmp[1] >> tmp[2];

        //auto& s = lclMatrix.lclScaling;
        //s.x = tmp[mCoordAxis] * mCoordAxisSign;
        //s.y = tmp[mUpAxis] * mUpAxisSign;
        //s.z = tmp[mFrontAxis] * mFrontAxisSign;

        lclMatrix.lclMatrix = Matrix4::createScale(s);
    }
    if (lclModelObject.hasProperties("Lcl Rotation")) {
        const auto& value = lclModelObject.getProperties("Lcl Rotation").value;
        std::istringstream iss(value);

        Vector3 euler;
        iss >> euler.x >> euler.y >> euler.z;

        //float tmp[3] = { 0.f, 0.f, 0.f };
        //iss >> tmp[0] >> tmp[1] >> tmp[2];

        //Vector3 euler;
        //euler.x = tmp[mCoordAxis] * mCoordAxisSign;
        //euler.y = tmp[mUpAxis] * mUpAxisSign;
        //euler.z = tmp[mFrontAxis] * mFrontAxisSign;

        lclMatrix.lclRotation.setEuler(euler);

        lclMatrix.lclMatrix *= Matrix4::createFromQuaternion(lclMatrix.lclRotation);
    }
    if (lclModelObject.hasProperties("Lcl Translation")) {
        const auto& value = lclModelObject.getProperties("Lcl Translation").value;
        std::istringstream iss(value);

        auto& t = lclMatrix.lclTranslation;
        iss >> t.x >> t.y >> t.z;

        //float tmp[3] = { 0.f, 0.f, 0.f };
        //iss >> tmp[0] >> tmp[1] >> tmp[2];

        //auto& t = lclMatrix.lclTranslation;
        //t.x = tmp[mCoordAxis] * mCoordAxisSign;
        //t.y = tmp[mUpAxis] * mUpAxisSign;
        //t.z = tmp[mFrontAxis] * mFrontAxisSign;

        lclMatrix.lclMatrix *= Matrix4::createTranslation(t);
    }

    auto id = lclModelObject.getNodeId();
    mLclMatrixConnections.emplace(id, lclMatrix);
    mLclModelNodeIDMap.emplace(id, mLclModelNodeIDMap.size());
}

void FbxMesh::parseGeometry(
    const FbxObject& objectsObject,
    const std::unordered_multimap<unsigned, unsigned>& connections
) {
    const auto& children = objectsObject.children;
    auto geometrys = children.equal_range("Geometry");
    for (auto& g = geometrys.first; g != geometrys.second; ++g) {
        const auto& obj = g->second;
        unsigned nodeID = obj.getNodeId();
        //ConnectionsからノードIDと一致するものを取得する
        auto range = connections.equal_range(nodeID);
        for (auto& r = range.first; r != range.second; ++r) {
            //lcl行列のノードIDと一致するか
            auto itr = mLclMatrixConnections.find(r->second);
            if (itr == mLclMatrixConnections.end()) {
                continue;
            }

            //Geometry管理用マップに追加する
            auto index = mLclModelNodeIDMap[r->second];
            mGeometryNodeIDMap.emplace(nodeID, index);

            //デフォルトの頂点成分を解析する
            const auto& lclMatrix = itr->second;
            parseVertices(mVertices[index], obj, lclMatrix.lclMatrix);
            parseIndices(mIndices[index], obj);
            parseNormals(mNormals[index], obj, lclMatrix.lclRotation);
            parseUV(mUVs[index], mUVIndices[index], obj);
        }
    }
}

void FbxMesh::parseVertices(
    std::vector<Vector3>& out,
    const FbxObject& geometryObject,
    const Matrix4& lclMatrix
) {
    const auto& vertices = geometryObject.getArray("Vertices");
    auto size = vertices.size() / 3;
    out.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& v = out[i];
        auto idx = i * 3;
        v.x = std::stof(vertices[idx]);
        v.y = std::stof(vertices[idx + 1]);
        v.z = std::stof(vertices[idx + 2]);

        //v.x = std::stof(vertices[idx + mCoordAxis]) * mCoordAxisSign;
        //v.y = std::stof(vertices[idx + mUpAxis]) * mUpAxisSign;
        //v.z = std::stof(vertices[idx + mFrontAxis]) * mFrontAxisSign;

        v = Vector3::transform(v, lclMatrix);
    }
}

void FbxMesh::parseIndices(
    std::vector<unsigned short>& out,
    const FbxObject& geometryObject
) {
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

void FbxMesh::parseNormals(
    std::vector<Vector3>& out,
    const FbxObject& geometryObject,
    const Quaternion& lclRotation
) {
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

        //n.x = std::stof(normals[idx + mCoordAxis]) * mCoordAxisSign;
        //n.y = std::stof(normals[idx + mUpAxis]) * mUpAxisSign;
        //n.z = std::stof(normals[idx + mFrontAxis]) * mFrontAxisSign;

        n = Vector3::transform(n, lclRotation);
    }
}

void FbxMesh::parseUV(
    std::vector<Vector2>& outUV,
    std::vector<unsigned short>& outUVIndices,
    const FbxObject& geometryObject
) {
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
