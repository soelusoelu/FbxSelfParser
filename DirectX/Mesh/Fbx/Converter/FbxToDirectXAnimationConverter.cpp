#include "FbxToDirectXAnimationConverter.h"
#include "../FbxAnimation.h"
#include "../FbxAnimationTime.h"
#include "../FbxBone.h"

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

        //キーフレームの読み込み
        loadAllKeyFrames(motion, bones, time);
    }
}

void FbxToDirectXAnimationConverter::loadAllKeyFrames(
    Motion& motion,
    const std::vector<Bone>& bones,
    const FbxAnimationTime& animationTime
) const {
    //ボーン数分拡張しとく
    motion.frameMatrix.resize(bones.size());

    //親のいないボーン(ルートボーン)を探す
    const Bone* rootBone = nullptr;
    for (const auto& b : bones) {
        if (!b.parent) {
            rootBone = &b;
            //キーフレームの添字は配列の最後にあるnullボーンにするために
            //keyFrameDataの最後の添字、ボーン+1の位置のためボーンの数
            auto keyFrameIndex = bones.size();
            loadKeyFrames(
                motion,
                nullptr,
                mAnimationParser.getKeyFrameData(keyFrameIndex),
                animationTime,
                rootBone->number
            );

            break;
        }
    }

    //ルートボーンからキーフレームを読み込み、子に降りていく
    loadChildrenKeyFrames(motion, *rootBone, animationTime);
}

void FbxToDirectXAnimationConverter::loadChildrenKeyFrames(
    Motion& motion,
    const Bone& bone,
    const FbxAnimationTime& animationTime
) const {
    auto boneIndex = bone.number;
    const std::vector<Matrix4>* parentFrameMatrix = nullptr;
    if (bone.parent) {
        parentFrameMatrix = &motion.frameMatrix[bone.parent->number];
    }

    //キーフレーム読み込み
    loadKeyFrames(
        motion,
        parentFrameMatrix,
        mAnimationParser.getKeyFrameData(boneIndex),
        animationTime,
        boneIndex
    );

    for (const auto& c : bone.children) {
        //ボーンの子で再帰呼び出し
        loadChildrenKeyFrames(motion, *c, animationTime);
    }
}

void FbxToDirectXAnimationConverter::loadKeyFrames(
    Motion& motion,
    const std::vector<Matrix4>* parentFrameMatrix,
    const KeyFrameData& keyFrames,
    const FbxAnimationTime& animationTime,
    unsigned boneIndex
) const {
    auto& m = motion.frameMatrix[boneIndex];
    auto numFrame = motion.numFrame;
    m.resize(numFrame);

    //フレーム数分フレーム時姿勢を取得する
    for (int i = 0; i < numFrame; ++i) {
        //指定フレームでの時間を取得する
        auto time = animationTime.getTime(i);
        //指定フレームでの姿勢
        const Matrix4* parentMat = nullptr;
        if (parentFrameMatrix) {
            parentMat = &parentFrameMatrix->at(i);
        }
        calcKeyFrame(m[i], parentMat, keyFrames, time, i);
    }
}

void FbxToDirectXAnimationConverter::calcKeyFrame(
    Matrix4& out,
    const Matrix4* parent,
    const KeyFrameData& keyFrames,
    long long time,
    int frame
) const {
    auto t = Vector3::zero;
    auto r = Vector3::zero;
    auto s = Vector3::one;
    calcKeyFrameValues(t, r, s, keyFrames, time, frame);

    out *= Matrix4::createScale(s);
    out *= Matrix4::createFromQuaternion(Quaternion(r));
    out *= Matrix4::createTranslation(t);

    //親がいるなら親の行列も掛け合わせる
    if (parent) {
        out *= *parent;
    }
}

void FbxToDirectXAnimationConverter::calcKeyFrameValues(
    Vector3& t,
    Vector3& r,
    Vector3& s,
    const KeyFrameData& keyFrames,
    long long time,
    int frame
) const {
    calcKeyFrameValue(t.x, keyFrames, frame, time, 0, 0); //tx
    calcKeyFrameValue(t.y, keyFrames, frame, time, 0, 1); //ty
    calcKeyFrameValue(t.z, keyFrames, frame, time, 0, 2); //tz
    calcKeyFrameValue(r.x, keyFrames, frame, time, 1, 0); //rx
    calcKeyFrameValue(r.y, keyFrames, frame, time, 1, 1); //ry
    calcKeyFrameValue(r.z, keyFrames, frame, time, 1, 2); //rz
    calcKeyFrameValue(s.x, keyFrames, frame, time, 2, 0); //sx
    calcKeyFrameValue(s.y, keyFrames, frame, time, 2, 1); //sy
    calcKeyFrameValue(s.z, keyFrames, frame, time, 2, 2); //sz
}

void FbxToDirectXAnimationConverter::calcKeyFrameValue(
    float& out,
    const KeyFrameData& keyFrames,
    int frame,
    long long time,
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

    //キーフレームがフレーム数分無い場合、timeから値を補間して取得する
    int idx = 1;
    for (; idx < times.size(); ++idx) {
        if (times[idx] > time) {
            break;
        }
    }

    auto aT = static_cast<long double>(times[idx - 1]);
    auto bT = static_cast<long double>(times[idx]);
    auto f = static_cast<float>(aT / bT);

    auto aV = values[idx - 1];
    auto bV = values[idx];

    out = Math::lerp(aV, bV, f);
}
