#include "FbxBone.h"
#include <cassert>

FbxBone::FbxBone(const FbxObject& objectsObject, const FbxObject& connectionsObject)
    : mObjectsObject(objectsObject)
    , mConnectionsObject(connectionsObject)
{
}

FbxBone::~FbxBone() = default;

void FbxBone::parse(std::vector<Bone>& bones) {
    if (mObjectsObject.hasObject("Pose")) {
        const auto& poseObject = mObjectsObject.getObject("Pose");
        parseBone(bones, poseObject);
        connect(bones);
    }
}

void FbxBone::parseBone(std::vector<Bone>& bones, const FbxObject& poseObject) {
    const auto& poseNodes = poseObject.getValue("NbPoseNodes");
    int boneCount = std::stoi(poseNodes) - 1;
    bones.resize(boneCount);

    const auto& children = poseObject.children;
    for (int i = 0; i < boneCount; ++i) {
        auto& bone = bones[i];
        const auto& c = children[i + 1];

        assert(c.name == "PoseNode");

        //for文のインデックスをそのままボーンの番号に
        bone.number = static_cast<unsigned short>(i);

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

        //ノード番号を取得する
        unsigned nodeNo = static_cast<unsigned>(std::stoi(c.getValue("Node")));

        mConnections.emplace(nodeNo, i);
    }
}

void FbxBone::connect(std::vector<Bone>& bones) {
    const auto& connections = mConnectionsObject.connections;
    for (const auto& c : connections) {
        //子の番号が全ボーンのノード番号と一致しなければ次へ
        if (mConnections.find(c.child) == mConnections.end()) {
            continue;
        }
        //親の番号が全ボーンのノード番号と一致しなければ次へ
        if (mConnections.find(c.parent) == mConnections.end()) {
            continue;
        }

        auto childNo = mConnections[c.child];
        auto parentNo = mConnections[c.parent];

        auto& child = bones[childNo];
        auto& parent = bones[parentNo];

        child.parent = &parent;
        parent.children.emplace_back(&child);
    }
}
