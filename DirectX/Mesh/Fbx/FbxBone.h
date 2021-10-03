#pragma once

#include "Reader/FbxObject.h"
#include "../IMeshLoader.h"
#include "../../Math/Math.h"
#include <memory>
#include <string>
#include <unordered_map>

class FbxWeight;

struct BoneData {
    unsigned short boneIndex = 0;
    std::string name;
    Matrix4 initMatrix = Matrix4::identity;
};

class FbxBone {
public:
    FbxBone(const FbxObject& objectsObject);
    ~FbxBone();
    FbxBone(const FbxBone&) = delete;
    FbxBone& operator=(const FbxBone&) = delete;

    const FbxWeight& getWeightParser() const;
    const std::unordered_map<unsigned, BoneData>& getBoneData() const;
    bool hasBone() const;
    unsigned getBoneCount() const;

private:
    void parseLimbNode();
    void parsePose(const FbxObject& poseObject);

private:
    const FbxObject& mObjectsObject;
    std::unique_ptr<FbxWeight> mWeightParser;
    //key: LimbNodeノード番号、value: ボーン情報
    std::unordered_map<unsigned, BoneData> mBoneData;
};
