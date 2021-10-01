#pragma once

#include "FbxAnimationTime.h"
#include "Reader/FbxObject.h"
#include "../Bone.h"
#include "../Motion.h"
#include "../../Math/Math.h"
#include <unordered_map>
#include <vector>

class FbxBone;

class FbxAnimation {
    enum class TRS {
        T, R, S
    };

    struct AnimationCurveNode {
        Vector3 value;
        TRS type;
    };

    //キーフレームに関するデータ
    struct KeyFrameData {
        //t, r, sの3成分のそれぞれx, y, z
        std::vector<float> values[3][3];
        std::vector<long long> times[3][3];
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

    void parse(std::vector<Motion>& motions, const std::vector<Bone>& bones);

private:
    void parseTime();
    void parseAnimationCurveNode();
    void parseAnimationStack(std::vector<Motion>& motions);
    //全キーフレームを読み込む
    void loadAllKeyFrames(Motion& motion, unsigned boneCount);
    //キーフレーム読み込み
    void loadKeyFrames(Motion& motion, unsigned boneIndex);
    //キーフレームを事前に読み込む
    void preloadKeyFrames(int boneIndex);
    //キーフレームに関するデータを取得する
    void getKeyFrameData(
        KeyFrameData& out,
        const Vector3& t,
        const Vector3& r,
        const Vector3& s,
        TRS type,
        const std::string& xyz,
        unsigned childNo
    ) const;
    //FbxValueArrayからvector<float>に変換する
    void convertKeyValues(std::vector<float>& dst, const FbxValueArray& src) const;
    //FbxValueArrayからvector<long long>に変換する
    void convertKeyTimes(std::vector<long long>& dst, const FbxValueArray& src) const;
    //キーフレームを求める
    void calcKeyFrame(Matrix4& out, int frame, unsigned boneIndex) const;
    void calcKeyFrameValues(
        Vector3& t,
        Vector3& r,
        Vector3& s,
        const KeyFrameData& data,
        int frame
    ) const;
    void calcKeyFrameValue(
        float& out,
        const KeyFrameData& data,
        int frame,
        long long time,
        int trs,
        int xyz
    ) const;

private:
    const FbxObject& mGlobalSettingsObject;
    const FbxObject& mObjectsObject;
    const FbxBone& mBoneParser;
    const std::vector<FbxConnections>& mConnections;
    FbxAnimationTime mAnimationTime;

    std::unordered_map<unsigned, AnimationCurveNode> mAnimationCurveNode;
    unsigned mAnimationLayer;

    //ボーンの数のキーフレームに関するデータ
    std::vector<KeyFrameData> mKeyFrames;
};
