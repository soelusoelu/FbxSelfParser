#include "FbxAnimation.h"
#include <string>

FbxAnimation::FbxAnimation(const FbxObject& globalSettingsObject, const FbxObject& objectsObject)
    : mGlobalSettingsObject(globalSettingsObject)
    , mObjectsObject(objectsObject)
    , mAnimationTime()
{
    parseTime();
}

FbxAnimation::~FbxAnimation() = default;

void FbxAnimation::parse(std::vector<Motion>& motions, const std::vector<Bone>& bones) {
    parseAnimationStack(motions);
    auto motionCount = motions.size();
    for (size_t i = 0; i < motionCount; ++i) {
        auto& motion = motions[i];

        //モーションのフレーム数を設定
        motion.numFrame = mAnimationTime.stopFrame - mAnimationTime.startFrame;

        //キーフレームの読み込み
        loadAllKeyFrames(motion, bones.size());
    }
}

void FbxAnimation::parseTime() {
    //TimeModeを取得する
    const auto& timeMode = mGlobalSettingsObject.getProperties("TimeMode").value;
    mAnimationTime.timeMode = static_cast<FbxTimeMode>(std::stoi(timeMode));

    //TimeSpanStartを取得する
    const auto& timeSpanStart = mGlobalSettingsObject.getProperties("TimeSpanStart").value;
    mAnimationTime.timeSpanStart = std::stoll(timeSpanStart);

    //TimeSpanStopを取得する
    const auto& timeSpanStop = mGlobalSettingsObject.getProperties("TimeSpanStop").value;
    mAnimationTime.timeSpanStop = std::stoll(timeSpanStop);

    //TimeModeから各種時間を求める
    if (mAnimationTime.timeMode == FbxTimeMode::FRAMES_24) {
        mAnimationTime.start = mAnimationTime.timeSpanStart / 24;
        mAnimationTime.stop = mAnimationTime.timeSpanStop / 24;
        mAnimationTime.period = mAnimationTime.stop / 24;
    }

    //開始・終了フレームを求める
    mAnimationTime.startFrame = static_cast<int>(mAnimationTime.start / mAnimationTime.period);
    mAnimationTime.stopFrame = static_cast<int>(mAnimationTime.stop / mAnimationTime.period);
}

void FbxAnimation::parseAnimationStack(std::vector<Motion>& motions) {
    const auto& children = mObjectsObject.children;
    auto range = children.equal_range("AnimationStack");
    for (auto& itr = range.first; itr != range.second; ++itr) {
        auto& motion = motions.emplace_back();
        motion.name = itr->second.attributes[1].substr(11); //11はAnimStack::の文字数分
    }
}

void FbxAnimation::loadAllKeyFrames(Motion& motion, unsigned boneCount) {
    //ボーン数分拡張しとく
    motion.frameMatrix.resize(boneCount);
    for (unsigned i = 0; i < boneCount; ++i) {
        //キーフレーム読み込み
        loadKeyFrames(motion, i);
    }
}

void FbxAnimation::loadKeyFrames(Motion& motion, unsigned boneIndex) {
    auto& m = motion.frameMatrix[boneIndex];
    m.resize(motion.numFrame);

    //フレーム数分フレーム時姿勢を取得する
    for (int i = 0; i < motion.numFrame; ++i) {
        //指定フレームでの時間を取得する
        const auto& time = mAnimationTime.getTime(i);
        //指定フレームでの姿勢
        //m[i] = 
    }
}
