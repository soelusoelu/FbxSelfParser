#include "FbxToDirectXAnimationConverter.h"
#include "../FbxAnimation.h"
#include "../FbxAnimationTime.h"
#include "../FbxBone.h"
#include <cassert>

FbxToDirectXAnimationConverter::FbxToDirectXAnimationConverter(
    const FbxBone& boneParser,
    const FbxAnimation& animationParser,
    const std::unordered_multimap<unsigned, unsigned>& connections
)
    : mBoneParser(boneParser)
    , mAnimationParser(animationParser)
    , mConnections(connections)
{
}

FbxToDirectXAnimationConverter::~FbxToDirectXAnimationConverter() = default;

void FbxToDirectXAnimationConverter::convert(
    std::vector<Bone>& bones,
    std::vector<Motion>& motions
) const {
    //ボーンがないなら終了
    if (mBoneParser.getBoneCount() == 0) {
        return;
    }

    //ボーンを変換する
    convertBone(bones);

    //アニメーションを変換する
    convertAnimation(motions, bones);
}

void FbxToDirectXAnimationConverter::convertBone(
    std::vector<Bone>& bones
) const {
    bones.resize(mBoneParser.getBoneCount());
    const auto& boneData = mBoneParser.getBoneData();
    for (const auto& bd : boneData) {
        const auto& data = bd.second;
        auto boneIndex = data.boneIndex;

        //必要な要素を設定していく
        auto& bone = bones[boneIndex];
        //ボーンの番号を設定する
        bone.number = boneIndex;
        //名前を設定する
        bone.name = data.name;
        //初期姿勢をコピーする
        memcpy(&bone.initMat, &data.initMatrix, sizeof(Matrix4));
        //初期姿勢からオフセット行列を求める
        bone.offsetMat = Matrix4::inverse(bone.initMat);

        //親子関係を構築する
        auto range = mConnections.equal_range(bd.first);
        for (auto& itr = range.first; itr != range.second; ++itr) {
            auto parentNode = boneData.find(itr->second);
            if (parentNode != boneData.end()) {
                auto parentIndex = parentNode->second.boneIndex;
                auto& parent = bones[parentIndex];
                bone.parent = &parent;
                parent.children.emplace_back(&bone);
            }
        }
    }
}

void FbxToDirectXAnimationConverter::convertAnimation(
    std::vector<Motion>& motions,
    const std::vector<Bone>& bones
) const {
    auto animationCount = mAnimationParser.getAnimationCount();
    motions.resize(animationCount);

    for (size_t i = 0; i < animationCount; ++i) {
        auto& motion = motions[i];

        const auto& stack = mAnimationParser.getAnimationStack(i);
        //モーション名を設定する
        motion.name = stack.name;
        //モーションのフレーム数を設定
        const auto& time = stack.time;
        motion.numFrame = time.stopFrame - time.startFrame;

        //キーフレームに関するデータを取得
        const auto& keyFrames = mAnimationParser.getKeyFramesData(i);

        //キーフレームの読み込み
        loadAllKeyFrames(motion, bones, time, keyFrames);
    }
}

void FbxToDirectXAnimationConverter::loadAllKeyFrames(
    Motion& motion,
    const std::vector<Bone>& bones,
    const FbxAnimationTime& animationTime,
    const std::vector<KeyFrameData>& keyFrames
) const {
    //事前に拡張しとく
    auto numFrame = motion.numFrame;
    motion.frameMatrix.resize(bones.size());
    for (auto&& fm : motion.frameMatrix) {
        fm.resize(numFrame);
    }

    //親のいないボーン(ルートボーン)を探す
    const Bone* rootBone = nullptr;
    for (const auto& b : bones) {
        if (!b.parent) {
            rootBone = &b;
            break;
        }
    }

    assert(rootBone);

    //Armatureノードがあればキーフレームを取得する
    const KeyFrameData* armKeyFrames = nullptr;
    if (mBoneParser.hasArmatureNode()) {
        armKeyFrames = &mAnimationParser.getArmatureKeyFrameData();
    }

    //フレーム数分フレーム時姿勢を取得する
    for (int i = 0; i < numFrame; ++i) {
        //指定フレームでの時間を取得する
        auto time = animationTime.getTime(i);

        //Armatureの行列を求める
        Matrix4 arm;
        if (armKeyFrames) {
            calcKeyFrame(arm, *armKeyFrames, time, i);
        }

        //ルートボーンからキーフレームを読み込み、子に降りていく
        loadChildrenKeyFrames(motion, *rootBone, keyFrames, arm, time, i);
    }
}

void FbxToDirectXAnimationConverter::loadChildrenKeyFrames(
    Motion& motion,
    const Bone& bone,
    const std::vector<KeyFrameData>& keyFrames,
    const Matrix4& armatureMatrix,
    long long time,
    int frame
) const {
    auto boneIndex = bone.number;

    //キーフレーム読み込み
    calcKeyFrame(
        motion.frameMatrix[boneIndex][frame],
        keyFrames[boneIndex],
        time,
        frame
    );

    //ボーンの子で再帰呼び出し
    for (const auto& c : bone.children) {
        loadChildrenKeyFrames(motion, *c, keyFrames, armatureMatrix, time, frame);
    }

    //親がいるなら親の行列も掛け合わせる
    auto parent = bone.parent;
    auto& fm = motion.frameMatrix;
    while (parent) {
        fm[boneIndex][frame] *= fm[parent->number][frame];
        parent = parent->parent;
    }

    //最後にArmature行列を掛ける
    fm[boneIndex][frame] *= armatureMatrix;
}

void FbxToDirectXAnimationConverter::calcKeyFrame(
    Matrix4& out,
    const KeyFrameData& keyFrames,
    long long time,
    int frame
) const {
    auto t = Vector3::zero;
    auto r = Vector3::zero;
    auto s = Vector3::one;
    calcKeyFrameValues(t, r, s, keyFrames, time, frame);

    out = Matrix4::createScale(s);
    out *= Matrix4::createFromQuaternion(Quaternion(r));
    out *= Matrix4::createTranslation(t);
}

void FbxToDirectXAnimationConverter::calcKeyFrameValues(
    Vector3& t,
    Vector3& r,
    Vector3& s,
    const KeyFrameData& keyFrames,
    long long time,
    int frame
) const {
    if (mAnimationParser.hasKeyFrameData(keyFrames, 0, 0)) {
        calcKeyFrameValue(t.x, keyFrames, time, frame, 0, 0); //tx
    }
    if (mAnimationParser.hasKeyFrameData(keyFrames, 0, 1)) {
        calcKeyFrameValue(t.y, keyFrames, time, frame, 0, 1); //ty
    }
    if (mAnimationParser.hasKeyFrameData(keyFrames, 0, 2)) {
        calcKeyFrameValue(t.z, keyFrames, time, frame, 0, 2); //tz
    }

    if (mAnimationParser.hasKeyFrameData(keyFrames, 1, 0)) {
        calcKeyFrameValue(r.x, keyFrames, time, frame, 1, 0); //rx
    }
    if (mAnimationParser.hasKeyFrameData(keyFrames, 1, 1)) {
        calcKeyFrameValue(r.y, keyFrames, time, frame, 1, 1); //ry
    }
    if (mAnimationParser.hasKeyFrameData(keyFrames, 1, 2)) {
        calcKeyFrameValue(r.z, keyFrames, time, frame, 1, 2); //rz
    }

    if (mAnimationParser.hasKeyFrameData(keyFrames, 2, 0)) {
        calcKeyFrameValue(s.x, keyFrames, time, frame, 2, 0); //sx
    }
    if (mAnimationParser.hasKeyFrameData(keyFrames, 2, 1)) {
        calcKeyFrameValue(s.y, keyFrames, time, frame, 2, 1); //sy
    }
    if (mAnimationParser.hasKeyFrameData(keyFrames, 2, 2)) {
        calcKeyFrameValue(s.z, keyFrames, time, frame, 2, 2); //sz
    }
}

void FbxToDirectXAnimationConverter::calcKeyFrameValue(
    float& out,
    const KeyFrameData& keyFrames,
    long long time,
    int frame,
    int trs,
    int xyz
) const {
    const auto& keyValues = keyFrames.values;
    const auto& keyTimes = keyFrames.times;

    const auto& values = keyValues[trs][xyz];
    const auto& times = keyTimes[trs][xyz];
    //値が1つしかないならそれを使う
    auto size = values.size();
    if (size == 1) {
        out = values[0];
        return;
    }
    if (frame < size) {
        //指定フレームでの時間が一致してればその値
        if (times[frame] == time) {
            out = values[frame];
            return;
        }
    }

    //キーフレームがフレーム数分無い、または一致しない場合timeから値を補間して取得する
    //int idx = 1;
    //for (; idx < times.size(); ++idx) {
    //    if (times[idx] > time) {
    //        break;
    //    }
    //}

    //auto aT = static_cast<long double>(times[idx - 1]);
    //auto bT = static_cast<long double>(times[idx]);
    //auto f = static_cast<float>(aT / bT);

    //auto aV = values[idx - 1];
    //auto bV = values[idx];

    //out = Math::lerp(aV, bV, f);

    auto aT = static_cast<long double>(mAnimationParser.getTimeModeTime() * frame);
    auto bT = static_cast<long double>(times.back());
    auto f = static_cast<float>(aT / bT);

    auto aV = values.front();
    auto bV = values.back();

    out = Math::lerp(aV, bV, f);
}
