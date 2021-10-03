#pragma once

#include "FbxAnimationTime.h"
#include "Reader/FbxObject.h"
#include "../../Math/Math.h"
#include <string>
#include <unordered_map>
#include <vector>

class FbxBone;

struct AnimationStack {
    std::string name;
    FbxAnimationTime time;
};

//キーフレームに関するデータ
struct KeyFrameData {
    //t, r, sの3成分のそれぞれx, y, z
    std::vector<float> values[3][3];
    std::vector<long long> times[3][3];
};

class FbxAnimation {
    enum class TRS {
        T, R, S
    };

    struct AnimationCurveNode {
        unsigned targetBoneNodeId = 0;
        unsigned tNodeId = 0;
        unsigned rNodeId = 0;
        unsigned sNodeId = 0;
        Vector3 t = Vector3::zero;
        Vector3 r = Vector3::zero;
        Vector3 s = Vector3::one;
    };

public:
    FbxAnimation(
        const FbxObject& globalSettingsObject,
        const FbxObject& objectsObject,
        const FbxBone& boneParser,
        const std::vector<FbxConnections>& connections
    );
    ~FbxAnimation();
    FbxAnimation(const FbxAnimation&) = delete;
    FbxAnimation& operator=(const FbxAnimation&) = delete;

    //AnimationStackを取得する
    const AnimationStack& getAnimationStack(unsigned index) const;
    //キーフレームに関するデータを取得する
    const KeyFrameData& getKeyFrameData(unsigned boneIndex) const;
    //アニメーション数を取得する
    unsigned getAnimationCount() const;

private:
    void parseAnimationStack();
    void parseGlobalSettingsTime(const FbxObject& globalSettingsObject);
    void parseTime();
    void parseAnimationCurveNode(const FbxBone& boneParser);
    //キーフレームを事前に読み込む
    void preloadKeyFrames(int boneIndex);
    //キーフレームに関するデータを取得する
    void getKeyFrameData(
        KeyFrameData& out,
        const AnimationCurveNode& animationCurveNode,
        TRS type,
        const std::string& xyz,
        unsigned childNo
    ) const;
    //FbxValueArrayからvector<float>に変換する
    void convertKeyValues(std::vector<float>& dst, const FbxValueArray& src) const;
    //FbxValueArrayからvector<long long>に変換する
    void convertKeyTimes(std::vector<long long>& dst, const FbxValueArray& src) const;

private:
    const FbxObject& mObjectsObject;
    const std::vector<FbxConnections>& mConnections;
    //アニメーション配列
    std::vector<AnimationStack> mAnimationStacks;
    //ボーンのインデックス順
    std::vector<AnimationCurveNode> mAnimationCurveNode;
    //ボーンの数のキーフレームに関するデータ
    std::vector<KeyFrameData> mKeyFrames;

    //グローバルタイム
    FbxGlobalTime mGlobalTime;
};
