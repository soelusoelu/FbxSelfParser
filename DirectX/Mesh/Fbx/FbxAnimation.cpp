#include "FbxAnimation.h"
#include "FbxBone.h"
#include <cassert>

FbxAnimation::FbxAnimation(
    const FbxObject& globalSettingsObject,
    const FbxObject& objectsObject,
    const FbxBone& boneParser,
    const std::vector<FbxConnections>& connections
)
    : mObjectsObject(objectsObject)
    , mConnections(connections)
    , mGlobalTime()
{
    //ボーン数
    unsigned boneCount = boneParser.getBoneCount();

    //ボーンがないなら終了
    if (boneCount == 0) {
        return;
    }

    parseAnimationStack();

    //アニメーションがなければ終了
    if (getAnimationCount() == 0) {
        return;
    }

    parseGlobalSettingsTime(globalSettingsObject);
    parseTime();
    parseAnimationCurveNode(boneParser);

    const auto& boneData = boneParser.getBoneData();
    mKeyFrames.resize(boneCount);
    for (const auto& b : boneData) {
        //キーフレームに関するデータを事前に読み込んでおく
        preloadKeyFrames(b.second.boneIndex);
    }
}

FbxAnimation::~FbxAnimation() = default;

const AnimationStack& FbxAnimation::getAnimationStack(unsigned index) const {
    assert(index < getAnimationCount());
    return mAnimationStacks[index];
}

const KeyFrameData& FbxAnimation::getKeyFrameData(unsigned boneIndex) const {
    assert(boneIndex < mKeyFrames.size());
    return mKeyFrames[boneIndex];
}

unsigned FbxAnimation::getAnimationCount() const {
    return mAnimationStacks.size();
}

void FbxAnimation::parseAnimationStack() {
    const auto& children = mObjectsObject.children;
    auto range = children.equal_range("AnimationStack");
    for (auto& itr = range.first; itr != range.second; ++itr) {
        const auto& obj = itr->second;
        auto& stack = mAnimationStacks.emplace_back();

        //アニメーション名を取得する
        stack.name = obj.attributes[1].substr(11); //11はAnimStack::の文字数分

        //LocalStartを取得する
        if (obj.hasProperties("LocalStart")) {
            const auto& localStart = obj.getProperties("LocalStart");
            stack.time.localStart = std::stoll(localStart.value);
        }

        //LocalStopを取得する
        const auto& localStop = obj.getProperties("LocalStop");
        stack.time.localStop = std::stoll(localStop.value);
    }
}

void FbxAnimation::parseGlobalSettingsTime(const FbxObject& globalSettingsObject) {
    //TimeModeを取得する
    const auto& timeMode = globalSettingsObject.getProperties("TimeMode").value;
    mGlobalTime.timeMode = static_cast<FbxTimeMode>(std::stoi(timeMode));

    //TimeSpanStartを取得する
    const auto& timeSpanStart = globalSettingsObject.getProperties("TimeSpanStart").value;
    mGlobalTime.timeSpanStart = std::stoll(timeSpanStart);

    //TimeSpanStopを取得する
    const auto& timeSpanStop = globalSettingsObject.getProperties("TimeSpanStop").value;
    mGlobalTime.timeSpanStop = std::stoll(timeSpanStop);
}

void FbxAnimation::parseTime() {
    for (auto&& stack : mAnimationStacks) {
        auto& time = stack.time;

        //TimeModeから各種時間を求める
        if (mGlobalTime.timeMode == FbxTimeMode::FRAMES_24) {
            time.period = mGlobalTime.timeSpanStop / 24;
        }

        //開始・終了フレームを求める
        time.startFrame = static_cast<int>(time.localStart / time.period);
        time.stopFrame = static_cast<int>(time.localStop / time.period);
    }
}

void FbxAnimation::parseAnimationCurveNode(const FbxBone& boneParser) {
    const auto& boneData = boneParser.getBoneData();
    mAnimationCurveNode.resize(boneData.size());

    auto nodes = mObjectsObject.children.equal_range("AnimationCurveNode");
    for (auto& itr = nodes.first; itr != nodes.second; ++itr) {
        const auto& obj = itr->second;

        auto nodeId = obj.getNodeId();
        for (const auto& c : mConnections) {
            if (c.child != nodeId) {
                continue;
            }
            auto f = boneData.find(c.parent);
            if (f == boneData.end()) {
                continue;
            }

            auto& node = mAnimationCurveNode[f->second.boneIndex];
            node.targetBoneNodeId = c.parent;

            auto attr = obj.attributes[1].substr(15); //15はAnimCurveNode::の文字数分
            if (attr == "T") {
                node.tNodeId = obj.getNodeId();

                if (obj.hasProperties("d|X")) {
                    node.t.x = std::stof(obj.getProperties("d|X").value);
                }
                if (obj.hasProperties("d|Y")) {
                    node.t.y = std::stof(obj.getProperties("d|Y").value);
                }
                if (obj.hasProperties("d|Z")) {
                    node.t.z = std::stof(obj.getProperties("d|Z").value);
                }
            } else if (attr == "R") {
                node.rNodeId = obj.getNodeId();

                if (obj.hasProperties("d|X")) {
                    node.r.x = std::stof(obj.getProperties("d|X").value);
                }
                if (obj.hasProperties("d|Y")) {
                    node.r.y = std::stof(obj.getProperties("d|Y").value);
                }
                if (obj.hasProperties("d|Z")) {
                    node.r.z = std::stof(obj.getProperties("d|Z").value);
                }
            } else if (attr == "S") {
                node.sNodeId = obj.getNodeId();

                if (obj.hasProperties("d|X")) {
                    node.s.x = std::stof(obj.getProperties("d|X").value);
                }
                if (obj.hasProperties("d|Y")) {
                    node.s.y = std::stof(obj.getProperties("d|Y").value);
                }
                if (obj.hasProperties("d|Z")) {
                    node.s.z = std::stof(obj.getProperties("d|Z").value);
                }
            }
        }
    }
}

void FbxAnimation::preloadKeyFrames(int boneIndex) {
    const auto& animationCurveNode = mAnimationCurveNode[boneIndex];

    //キーフレームの値を取得する
    auto& keyFrames = mKeyFrames[boneIndex];
    for (const auto& c : mConnections) {
        if (c.parent == animationCurveNode.tNodeId) {
            getKeyFrameData(keyFrames, animationCurveNode, TRS::T, c.value, c.child);
        }
        if (c.parent == animationCurveNode.rNodeId) {
            getKeyFrameData(keyFrames, animationCurveNode, TRS::R, c.value, c.child);
        }
        if (c.parent == animationCurveNode.sNodeId) {
            getKeyFrameData(keyFrames, animationCurveNode, TRS::S, c.value, c.child);
        }
    }
}

void FbxAnimation::getKeyFrameData(
    KeyFrameData& out,
    const AnimationCurveNode& animationCurveNode,
    TRS type,
    const std::string& xyz,
    unsigned childNo
) const {
    auto animationCurves = mObjectsObject.children.equal_range("AnimationCurve");
    for (auto& itr = animationCurves.first; itr != animationCurves.second; ++itr) {
        const auto& obj = itr->second;
        if (childNo != obj.getNodeId()) {
            continue;
        }

        const auto& values = obj.getArray("KeyValueFloat");
        const auto& times = obj.getArray("KeyTime");
        //TRS * XYZの9成分から一致するものを探す
        if (type == TRS::T) {
            if (xyz == "d|X") {
                convertKeyValues(out.values[0][0], values);
                convertKeyTimes(out.times[0][0], times);
                //for (auto&& v : out.values[0][0]) {
                //    v += t.x;
                //}
            } else if (xyz == "d|Y") {
                convertKeyValues(out.values[0][1], values);
                convertKeyTimes(out.times[0][1], times);
                //for (auto&& v : out.values[0][1]) {
                //    v += t.y;
                //}
            } else if (xyz == "d|Z") {
                convertKeyValues(out.values[0][2], values);
                convertKeyTimes(out.times[0][2], times);
                //for (auto&& v : out.values[0][2]) {
                //    v += t.z;
                //}
            }
        } else if (type == TRS::R) {
            if (xyz == "d|X") {
                convertKeyValues(out.values[1][0], values);
                convertKeyTimes(out.times[1][0], times);
                //for (auto&& v : out.values[1][0]) {
                //    v += r.x;
                //}
            } else if (xyz == "d|Y") {
                convertKeyValues(out.values[1][1], values);
                convertKeyTimes(out.times[1][1], times);
                //for (auto&& v : out.values[1][1]) {
                //    v += r.y;
                //}
            } else if (xyz == "d|Z") {
                convertKeyValues(out.values[1][2], values);
                convertKeyTimes(out.times[1][2], times);
                //for (auto&& v : out.values[1][2]) {
                //    v += r.z;
                //}
            }
        } else if (type == TRS::S) {
            if (xyz == "d|X") {
                convertKeyValues(out.values[2][0], values);
                convertKeyTimes(out.times[2][0], times);
                //for (auto&& v : out.values[2][0]) {
                //    v *= s.x;
                //}
            } else if (xyz == "d|Y") {
                convertKeyValues(out.values[2][1], values);
                convertKeyTimes(out.times[2][1], times);
                //for (auto&& v : out.values[2][1]) {
                //    v *= s.y;
                //}
            } else if (xyz == "d|Z") {
                convertKeyValues(out.values[2][2], values);
                convertKeyTimes(out.times[2][2], times);
                //for (auto&& v : out.values[2][2]) {
                //    v *= s.z;
                //}
            }
        }
    }
}

void FbxAnimation::convertKeyValues(std::vector<float>& dst, const FbxValueArray& src) const {
    auto size = src.size();
    dst.resize(size);
    for (size_t i = 0; i < size; ++i) {
        dst[i] = std::stof(src[i]);
    }
}

void FbxAnimation::convertKeyTimes(std::vector<long long>& dst, const FbxValueArray& src) const {
    auto size = src.size();
    dst.resize(size);
    for (size_t i = 0; i < size; ++i) {
        dst[i] = std::stoll(src[i]);
    }
}
