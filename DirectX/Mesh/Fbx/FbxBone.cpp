#include "FbxBone.h"
#include "FbxWeight.h"
#include <cassert>

FbxBone::FbxBone(const FbxObject& objectsObject)
    : mObjectsObject(objectsObject)
    , mWeightParser(nullptr)
{
    parseLimbNode();

    if (mObjectsObject.hasObject("Pose")) {
        const auto& poseObject = mObjectsObject.getObject("Pose");
        parsePose(poseObject);

        //ボーンがある場合のみ生成
        mWeightParser = std::make_unique<FbxWeight>(objectsObject);
    }
}

FbxBone::~FbxBone() = default;

const FbxWeight& FbxBone::getWeightParser() const {
    return *mWeightParser;
}

const std::unordered_map<unsigned, BoneData>& FbxBone::getBoneData() const {
    return mBoneData;
}

bool FbxBone::hasBone() const {
    return (mBoneData.size() > 0);
}

void FbxBone::parseLimbNode() {
    const auto& children = mObjectsObject.children;
    auto models = children.equal_range("Model");
    unsigned short count = 0;
    for (auto& itr = models.first; itr != models.second; ++itr) {
        const auto& obj = itr->second;
        const auto& attributes = obj.attributes;
        if (attributes[2] != "LimbNode") {
            continue;
        }

        BoneData data;
        data.boneIndex = count;
        data.name = attributes[1].substr(7); //7はModel::の文字数分

        mBoneData.emplace(obj.getNodeId(), data);

        ++count;
    }
}

void FbxBone::parsePose(const FbxObject& poseObject) {
    const auto& children = poseObject.children;

    auto nodes = children.equal_range("PoseNode");
    for (auto& itr = nodes.first; itr != nodes.second; ++itr) {
        const auto& obj = itr->second;

        //ノード番号を取得する
        unsigned nodeNo = static_cast<unsigned>(std::stoi(obj.getValue("Node")));

        //mBoneDataに含まれていなければ使用しないボーン(謎)
        auto f = mBoneData.find(nodeNo);
        if (f == mBoneData.end()) {
            continue;
        }

        auto& boneData = f->second;

        const auto& matrixArray = obj.getArray("Matrix");
        assert(matrixArray.size() == Matrix4::COLUMN_COUNT * Matrix4::ROW_COUNT);

        //初期姿勢を取得する
        auto& m = boneData.initMatrix.m;
        for (int col = 0; col < Matrix4::COLUMN_COUNT; ++col) {
            for (int row = 0; row < Matrix4::ROW_COUNT; ++row) {
                m[col][row] = std::stof(matrixArray[col * 4 + row]);
            }
        }
    }
}
