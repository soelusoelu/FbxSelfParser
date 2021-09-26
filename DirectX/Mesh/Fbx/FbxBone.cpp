#include "FbxBone.h"
#include "FbxWeight.h"
#include <cassert>

FbxBone::FbxBone(const FbxObject& objectsObject, const FbxObject& connectionsObject)
    : mObjectsObject(objectsObject)
    , mConnectionsObject(connectionsObject)
    , mWeightParser(std::make_unique<FbxWeight>(objectsObject))
{
}

FbxBone::~FbxBone() = default;

void FbxBone::parse(std::vector<Bone>& bones, MeshVertices& meshVertices, const Indices& indices, const FbxMesh& mesh) {
    if (mObjectsObject.hasObject("Pose")) {
        parseLimbNode(bones);

        const auto& poseObject = mObjectsObject.getObject("Pose");
        parseBone(bones, poseObject);

        connect(bones);

        mWeightParser->parse(meshVertices, indices, mesh);
    }
}

void FbxBone::parseLimbNode(std::vector<Bone>& bones) {
    const auto& children = mObjectsObject.children;
    for (const auto& c : children) {
        if (c.name != "Model") {
            continue;
        }
        const auto& attributes = c.attributes;
        if (attributes[2] != "LimbNode") {
            continue;
        }

        auto& bone = bones.emplace_back();
        auto boneNo = static_cast<unsigned short>(bones.size() - 1);
        bone.number = boneNo;
        bone.name = attributes[1].substr(7); //7はModel::の文字数分

        unsigned nodeNo = static_cast<unsigned>(std::stoi(attributes[0]));
        mConnections.emplace(nodeNo, boneNo);
    }
}

void FbxBone::parseBone(std::vector<Bone>& bones, const FbxObject& poseObject) const {
    const auto& children = poseObject.children;
    auto childCount = children.size();
    assert(std::stoi(poseObject.getValue("NbPoseNodes")) == childCount);

    for (size_t i = 0; i < childCount; ++i) {
        const auto& c = children[i];

        assert(c.name == "PoseNode");

        //ノード番号を取得する
        unsigned nodeNo = static_cast<unsigned>(std::stoi(c.getValue("Node")));

        //mConnectionsに含まれていなければ使用しないボーン(謎)
        auto itr = mConnections.find(nodeNo);
        if (itr == mConnections.end()) {
            continue;
        }

        auto& bone = bones[itr->second];

        const auto& matrixArray = c.getArray("Matrix");
        assert(matrixArray.size() == Matrix4::COLUMN_COUNT * Matrix4::ROW_COUNT);

        //初期姿勢を取得する
        auto& m = bone.initMat.m;
        for (int col = 0; col < Matrix4::COLUMN_COUNT; ++col) {
            for (int row = 0; row < Matrix4::ROW_COUNT; ++row) {
                m[col][row] = std::stof(matrixArray[col * 4 + row]);
            }
        }

        //初期姿勢からオフセット行列を求める
        bone.offsetMat = Matrix4::inverse(bone.initMat);
    }
}

void FbxBone::connect(std::vector<Bone>& bones) const {
    const auto& connections = mConnectionsObject.connections;
    for (const auto& c : connections) {
        //子の番号が全ボーンのノード番号と一致しなければ次へ
        auto itrC = mConnections.find(c.child);
        if (itrC == mConnections.end()) {
            continue;
        }
        //親の番号が全ボーンのノード番号と一致しなければ次へ
        auto itrP = mConnections.find(c.parent);
        if (itrP == mConnections.end()) {
            continue;
        }

        auto& child = bones[itrC->second];
        auto& parent = bones[itrP->second];

        child.parent = &parent;
        parent.children.emplace_back(&child);
    }
}
