#include "FbxAnimation.h"
#include "FbxBone.h"
#include <cassert>

FbxAnimation::FbxAnimation(
    const FbxObject& globalSettingsObject,
    const FbxObject& objectsObject,
    const FbxBone& boneParser,
    const std::unordered_multimap<unsigned, unsigned>& connections,
    const std::vector<FbxConnections>& connectionsVector
)
    : mObjectsObject(objectsObject)
    , mConnections(connections)
    , mConnectionsVector(connectionsVector)
    , mGlobalTime()
    , mArmatureAnimationCurveNode()
{
    //ボーン数
    unsigned boneCount = boneParser.getBoneCount();

    //ボーンがないなら終了
    if (boneCount == 0) {
        return;
    }

    parseGlobalSettingsTime(globalSettingsObject);
    parseAnimationStack();

    //アニメーションがなければ終了
    auto animCount = getAnimationCount();
    if (animCount == 0) {
        return;
    }

    parseAnimationLayer();
    parseAnimationCurveNode(boneParser);

    //キーフレームはボーン数
    mKeyFrames.resize(animCount);
    const auto& boneData = boneParser.getBoneData();
    for (size_t i = 0; i < animCount; ++i) {
        auto& kf = mKeyFrames[i];
        const auto& animCurve = mAnimationCurveNodes[i];
        for (const auto& b : boneData) {
            //キーフレームに関するデータを事前に読み込んでおく
            auto boneIndex = b.second.boneIndex;
            kf.resize(boneCount);
            preloadKeyFrames(kf[boneIndex], animCurve[boneIndex]);
        }
    }
    //Armatureに関するキーフレームを読み込む
    preloadKeyFrames(mArmatureKeyFrames, mArmatureAnimationCurveNode);
}

FbxAnimation::~FbxAnimation() = default;

const AnimationStack& FbxAnimation::getAnimationStack(unsigned index) const {
    for (const auto& stacks : mAnimationStacks) {
        const auto& stack = stacks.second;
        if (stack.animationNo == index) {
            return stack;
        }
    }
}

const std::vector<KeyFrameData>& FbxAnimation::getKeyFramesData(unsigned animationIndex) const {
    assert(animationIndex < getAnimationCount());
    return mKeyFrames[animationIndex];
}

bool FbxAnimation::hasKeyFrameData(const KeyFrameData& keyFrames, int trs, int xyz) const {
    assert(trs >= 0 && trs < 3 && xyz >= 0 && xyz < 3);
    return (keyFrames.values[trs][xyz].size() > 0);
}

const KeyFrameData& FbxAnimation::getArmatureKeyFrameData() const {
    return mArmatureKeyFrames;
}

unsigned FbxAnimation::getAnimationCount() const {
    return mAnimationStacks.size();
}

long long FbxAnimation::getTimeModeTime() const {
    if (mGlobalTime.timeMode == FbxTimeMode::FRAMES_24) {
        return FbxTimeModeTime::FRAMES_24_TIME;
    } else if (mGlobalTime.timeMode == FbxTimeMode::FRAMES_30) {
        return FbxTimeModeTime::FRAMES_30_TIME;
    } else if (mGlobalTime.timeMode == FbxTimeMode::FRAMES_60) {
        return FbxTimeModeTime::FRAMES_60_TIME;
    } else if (mGlobalTime.timeMode == FbxTimeMode::FRAMES_120) {
        return FbxTimeModeTime::FRAMES_120_TIME;
    }

    return 0;
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

void FbxAnimation::parseAnimationStack() {
    const auto& children = mObjectsObject.children;
    auto range = children.equal_range("AnimationStack");
    for (auto& itr = range.first; itr != range.second; ++itr) {
        const auto& obj = itr->second;

        AnimationStack stack;

        //アニメーション番号は自身を登録する前のサイズ
        stack.animationNo = mAnimationStacks.size();

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

        parseTime(stack.time);

        //AnimationStackを登録する
        mAnimationStacks.emplace(obj.getNodeId(), stack);
    }
}

void FbxAnimation::parseTime(FbxAnimationTime& time) {
    //TimeModeから1フレームごとの時間を取得する
    time.period = getTimeModeTime();

    //開始・終了フレームを求める
    time.startFrame = static_cast<int>(time.localStart / time.period);
    time.stopFrame = static_cast<int>(time.localStop / time.period);
}

void FbxAnimation::parseAnimationLayer() {
    auto layers = mObjectsObject.children.equal_range("AnimationLayer");
    for (auto& itr = layers.first; itr != layers.second; ++itr) {
        const auto& obj = itr->second;
        auto layerID = obj.getNodeId();
        for (const auto& c : mConnections) {
            if (c.first != layerID) {
                continue;
            }

            auto stack = mAnimationStacks.find(c.second);
            if (stack != mAnimationStacks.end()) {
                mAnimationLayers.emplace(layerID, stack->second.animationNo);
                break;
            }
        }
    }

    assert(mAnimationStacks.size() == mAnimationLayers.size());
}

void FbxAnimation::parseAnimationCurveNode(const FbxBone& boneParser) {
    const auto& boneData = boneParser.getBoneData();
    mAnimationCurveNodes.resize(getAnimationCount());
    for (auto&& node : mAnimationCurveNodes) {
        node.resize(boneData.size());
    }

    auto nodes = mObjectsObject.children.equal_range("AnimationCurveNode");
    for (auto& itr = nodes.first; itr != nodes.second; ++itr) {
        const auto& obj = itr->second;

        auto nodeId = obj.getNodeId();

        //事前準備
        AnimationCurveNode* node = nullptr;
        unsigned short animIndex = 0;
        unsigned short boneIndex = 0;
        unsigned boneNodeId = 0;
        bool isAnimLayer = false;
        bool isBone = false;
        bool isArmature = false;

        auto range = mConnections.equal_range(nodeId);
        for (auto& r = range.first; r != range.second; ++r) {
            auto parent = r->second;

            if (!isAnimLayer) {
                auto layerFind = mAnimationLayers.find(parent);
                if (layerFind != mAnimationLayers.end()) {
                    animIndex = layerFind->second;
                    isAnimLayer = true;
                    continue;
                }
            }
            if (!isBone) {
                auto boneFind = boneData.find(parent);
                if (boneFind != boneData.end()) {
                    boneIndex = boneFind->second.boneIndex;
                    boneNodeId = parent;
                    isBone = true;
                    continue;
                }
            }
            if (!isArmature) {
                if (boneParser.hasArmatureNode() && parent == boneParser.getArmatureNodeID()) {
                    node = &mArmatureAnimationCurveNode;
                    boneNodeId = parent;
                    isArmature = true;
                    continue;
                }
            }
        }

        if (!node) {
            assert(isAnimLayer && isBone);
            node = &mAnimationCurveNodes[animIndex][boneIndex];
        }

        node->targetBoneNodeId = boneNodeId;
        auto attr = obj.attributes[1].substr(15); //15はAnimCurveNode::の文字数分
        if (attr == "T") {
            node->tNodeId = nodeId;
        } else if (attr == "R") {
            node->rNodeId = nodeId;
        } else if (attr == "S") {
            node->sNodeId = nodeId;
        }
    }
}

void FbxAnimation::preloadKeyFrames(KeyFrameData& out, const AnimationCurveNode& animationCurveNode) {
    //キーフレームの値を取得する
    for (const auto& c : mConnectionsVector) {
        if (c.parent == animationCurveNode.tNodeId) {
            getKeyFrameData(out, TRS::T, c.value, c.child);
        }
        if (c.parent == animationCurveNode.rNodeId) {
            getKeyFrameData(out, TRS::R, c.value, c.child);
        }
        if (c.parent == animationCurveNode.sNodeId) {
            getKeyFrameData(out, TRS::S, c.value, c.child);
        }
    }
}

void FbxAnimation::getKeyFrameData(
    KeyFrameData& out,
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
            } else if (xyz == "d|Y") {
                convertKeyValues(out.values[0][1], values);
                convertKeyTimes(out.times[0][1], times);
            } else if (xyz == "d|Z") {
                convertKeyValues(out.values[0][2], values);
                convertKeyTimes(out.times[0][2], times);
            }
        } else if (type == TRS::R) {
            if (xyz == "d|X") {
                convertKeyValues(out.values[1][0], values);
                convertKeyTimes(out.times[1][0], times);
            } else if (xyz == "d|Y") {
                convertKeyValues(out.values[1][1], values);
                convertKeyTimes(out.times[1][1], times);
            } else if (xyz == "d|Z") {
                convertKeyValues(out.values[1][2], values);
                convertKeyTimes(out.times[1][2], times);
            }
        } else if (type == TRS::S) {
            if (xyz == "d|X") {
                convertKeyValues(out.values[2][0], values);
                convertKeyTimes(out.times[2][0], times);
            } else if (xyz == "d|Y") {
                convertKeyValues(out.values[2][1], values);
                convertKeyTimes(out.times[2][1], times);
            } else if (xyz == "d|Z") {
                convertKeyValues(out.values[2][2], values);
                convertKeyTimes(out.times[2][2], times);
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
