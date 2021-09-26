#pragma once

#include "FbxAnimationTime.h"
#include "Reader/FbxObject.h"
#include "../Bone.h"
#include "../Motion.h"
#include <vector>

class FbxAnimation {
public:
    FbxAnimation(const FbxObject& globalSettingsObject, const FbxObject& objectsObject);
    ~FbxAnimation();
    FbxAnimation(const FbxAnimation&) = delete;
    FbxAnimation& operator=(const FbxAnimation&) = delete;

    void parse(std::vector<Motion>& motions, const std::vector<Bone>& bones);

private:
    void parseTime();
    void parseAnimationStack(std::vector<Motion>& motions);
    //全キーフレームを読み込む
    void loadAllKeyFrames(Motion& motion, unsigned boneCount);
    //キーフレーム読み込み
    void loadKeyFrames(Motion& motion, unsigned boneIndex);

private:
    const FbxObject& mGlobalSettingsObject;
    const FbxObject& mObjectsObject;
    FbxAnimationTime mAnimationTime;
};
