#pragma once

#include "../FbxAnimationTime.h"
#include "../../Bone.h"
#include "../../Motion.h"
#include "../../../Math/Math.h"
#include <unordered_map>
#include <vector>

struct KeyFrameData;
class FbxBone;
class FbxAnimation;

class FbxToDirectXAnimationConverter {
public:
    FbxToDirectXAnimationConverter(
        const FbxBone& boneParser,
        const FbxAnimation& animationParser,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxToDirectXAnimationConverter();
    FbxToDirectXAnimationConverter(const FbxToDirectXAnimationConverter&) = delete;
    FbxToDirectXAnimationConverter& operator=(const FbxToDirectXAnimationConverter&) = delete;

    //アニメーション・ボーンをDirectXで求められる形に変換する
    void convert(
        std::vector<Bone>& bones,
        std::vector<Motion>& motions
    ) const;

private:
    //ボーンをDirectXで求められる形に変換する
    void convertBone(
        std::vector<Bone>& bones
    ) const;

    //アニメーションをDirectXで求められる形に変換する
    void convertAnimation(
        std::vector<Motion>& motions,
        const std::vector<Bone>& bones
    ) const;

    //全キーフレームを読み込む
    void loadAllKeyFrames(
        Motion& motion,
        const std::vector<Bone>& bones,
        const FbxAnimationTime& animationTime,
        const std::vector<KeyFrameData>& keyFrames
    ) const;

    //指定ボーンのキーフレームを読み込み、次に子ボーンのキーフレームを読み込んでいく
    void loadChildrenKeyFrames(
        Motion& motion,
        const Bone& bone,
        const std::vector<KeyFrameData>& keyFrames,
        const Matrix4& armatureMatrix,
        long long time,
        int frame
    ) const;

    //キーフレームを求める
    void calcKeyFrame(
        Matrix4& out,
        const KeyFrameData& keyFrames,
        long long time,
        int frame
    ) const;

    //指定フレームにおけるTRS・XYZのそれぞれの値を求める
    void calcKeyFrameValues(
        Vector3& t,
        Vector3& r,
        Vector3& s,
        const KeyFrameData& keyFrames,
        long long time,
        int frame
    ) const;

    //指定フレームにおける指定したTRS・XYZの値を求める
    void calcKeyFrameValue(
        float& out,
        const KeyFrameData& keyFrames,
        long long time,
        int frame,
        int trs,
        int xyz
    ) const;

private:
    const FbxBone& mBoneParser;
    const FbxAnimation& mAnimationParser;
    const std::unordered_multimap<unsigned, unsigned>& mConnections;
};
