#pragma once

#include "Reader/FbxObject.h"
#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include <memory>
#include <string>
#include <unordered_map>

class FbxMesh;
class FbxWeight;

struct BoneData {
    unsigned short boneIndex = 0;
    std::string name;
    Matrix4 initMatrix = Matrix4::identity;
};

class FbxBone {
public:
    FbxBone(
        const FbxObject& objectsObject,
        const FbxMesh& meshParser,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxBone();
    FbxBone(const FbxBone&) = delete;
    FbxBone& operator=(const FbxBone&) = delete;

    //ウェイトパーサーを取得する(ボーンがある場合のみ)
    const FbxWeight& getWeightParser() const;
    //ボーンデータをすべて取得する
    const std::unordered_map<unsigned, BoneData>& getBoneData() const;
    //ボーンがあるか
    bool hasBone() const;
    //ボーン数を取得する
    unsigned getBoneCount() const;
    //Armatureのノード番号
    unsigned getArmatureNodeID() const;
    //Armatureノードがあるか
    bool hasArmatureNode() const;

private:
    void parseLimbNode();
    void parseNullBone();
    void parsePose(const FbxObject& poseObject);

private:
    const FbxObject& mObjectsObject;
    std::unique_ptr<FbxWeight> mWeightParser;
    //key: LimbNodeノード番号、value: ボーン情報
    std::unordered_map<unsigned, BoneData> mBoneData;
    //Armatureノード番号
    unsigned mArmatureNodeId;
};
