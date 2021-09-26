#include "FbxBone.h"
#include "FbxWeight.h"
#include <cassert>

FbxBone::FbxBone(
    const FbxObject& objectsObject,
    const std::multimap<unsigned, unsigned>& connections
)
    : mObjectsObject(objectsObject)
    , mConnections(connections)
    , mWeightParser(std::make_unique<FbxWeight>(objectsObject))
{
}

FbxBone::~FbxBone() = default;

void FbxBone::parse(
    std::vector<Bone>& bones,
    std::vector<MeshVertices>& meshesVertices,
    const std::vector<Indices>& meshesIndices,
    const FbxMesh& mesh
) {
    if (mObjectsObject.hasObject("Pose")) {
        parseLimbNode(bones);

        const auto& poseObject = mObjectsObject.getObject("Pose");
        parseBone(bones, poseObject);

        connect(bones);

        mWeightParser->parse(meshesVertices, meshesIndices, mesh);
    }
}

void FbxBone::parseLimbNode(std::vector<Bone>& bones) {
    const auto& children = mObjectsObject.children;
    auto range = children.equal_range("Model");
    for (auto& r = range.first; r != range.second; ++r) {
        const auto& obj = r->second;
        const auto& attributes = obj.attributes;
        if (attributes[2] != "LimbNode") {
            continue;
        }

        auto& bone = bones.emplace_back();
        auto boneNo = static_cast<unsigned short>(bones.size() - 1);
        bone.number = boneNo;
        bone.name = attributes[1].substr(7); //7はModel::の文字数分

        unsigned nodeId = obj.getNodeId();
        mBoneConnections.emplace(nodeId, boneNo);
    }
}

void FbxBone::parseBone(std::vector<Bone>& bones, const FbxObject& poseObject) const {
    const auto& children = poseObject.children;
    auto childCount = children.size();
    assert(std::stoi(poseObject.getValue("NbPoseNodes")) == childCount);

    auto range = children.equal_range("PoseNode");
    for (auto& r = range.first; r != range.second; ++r) {
        const auto& obj = r->second;

        //ノード番号を取得する
        unsigned nodeNo = static_cast<unsigned>(std::stoi(obj.getValue("Node")));

        //mConnectionsに含まれていなければ使用しないボーン(謎)
        auto itr = mBoneConnections.find(nodeNo);
        if (itr == mBoneConnections.end()) {
            continue;
        }

        auto& bone = bones[itr->second];

        const auto& matrixArray = obj.getArray("Matrix");
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
    for (const auto& c : mConnections) {
        //子の番号が全ボーンのノード番号と一致しなければ次へ
        auto itrC = mBoneConnections.find(c.first);
        if (itrC == mBoneConnections.end()) {
            continue;
        }
        //親の番号が全ボーンのノード番号と一致しなければ次へ
        auto itrP = mBoneConnections.find(c.second);
        if (itrP == mBoneConnections.end()) {
            continue;
        }

        auto& child = bones[itrC->second];
        auto& parent = bones[itrP->second];

        child.parent = &parent;
        parent.children.emplace_back(&child);
    }
}
