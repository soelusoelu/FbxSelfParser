#include "FbxAnimation.h"
#include "FbxBone.h"
#include <string>

FbxAnimation::FbxAnimation(
    const FbxObject& globalSettingsObject,
    const FbxObject& objectsObject,
    const FbxBone& boneParser,
    const std::vector<FbxConnections>& connections
)
    : mGlobalSettingsObject(globalSettingsObject)
    , mObjectsObject(objectsObject)
    , mBoneParser(boneParser)
    , mConnections(connections)
    , mAnimationTime()
    , mAnimationLayer(0)
{
    parseTime();
    parseAnimationCurveNode();
}

FbxAnimation::~FbxAnimation() = default;

void FbxAnimation::parse(std::vector<Motion>& motions, const std::vector<Bone>& bones) {
    parseAnimationStack(motions);
    auto motionCount = motions.size();
    for (size_t i = 0; i < motionCount; ++i) {
        auto& motion = motions[i];

        mAnimationLayer = mObjectsObject.getObject("AnimationLayer").getNodeId();

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
        mAnimationTime.period = mAnimationTime.timeSpanStop / 24;
    }

    //LocalStartを取得する
    const auto& animationStack = mObjectsObject.getObject("AnimationStack");
    if (animationStack.hasProperties("LocalStart")) {
        const auto& localStart = animationStack.getProperties("LocalStart");
        mAnimationTime.localStart = std::stoll(localStart.value);
    }

    //LocalStopを取得する
    const auto& localStop = animationStack.getProperties("LocalStop");
    mAnimationTime.localStop = std::stoll(localStop.value);

    //開始・終了フレームを求める
    mAnimationTime.startFrame = static_cast<int>(mAnimationTime.localStart / mAnimationTime.period);
    mAnimationTime.stopFrame = static_cast<int>(mAnimationTime.localStop / mAnimationTime.period);
}

void FbxAnimation::parseAnimationCurveNode() {
    auto nodes = mObjectsObject.children.equal_range("AnimationCurveNode");
    for (auto& itr = nodes.first; itr != nodes.second; ++itr) {
        const auto& obj = itr->second;

        auto& node = mAnimationCurveNode.emplace(obj.getNodeId(), AnimationCurveNode{}).first->second;

        auto type = TRS::T;
        auto attr = obj.attributes[1].substr(15); //15はAnimCurveNode::の文字数分
        if (attr == "T") {
            node.type = TRS::T;
        } else if (attr == "R") {
            node.type = TRS::R;
        } else if (attr == "S") {
            node.type = TRS::S;
            node.value = Vector3::one;
        }

        if (obj.hasProperties("d|X")) {
            node.value.x = std::stof(obj.getProperties("d|X").value);
        }
        if (obj.hasProperties("d|Y")) {
            node.value.y = std::stof(obj.getProperties("d|Y").value);
        }
        if (obj.hasProperties("d|Z")) {
            node.value.z = std::stof(obj.getProperties("d|Z").value);
        }
    }
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
    mKeyFrames.resize(boneCount);
    for (unsigned i = 0; i < boneCount; ++i) {
        //キーフレーム読み込み
        loadKeyFrames(motion, i);
    }
}

void FbxAnimation::loadKeyFrames(Motion& motion, unsigned boneIndex) {
    auto& m = motion.frameMatrix[boneIndex];
    m.resize(motion.numFrame);

    //キーフレームに関するデータを事前に読み込んでおく
    preloadKeyFrames(boneIndex);

    //フレーム数分フレーム時姿勢を取得する
    for (int i = 0; i < motion.numFrame; ++i) {
        //指定フレームでの姿勢
        calcKeyFrame(m[i], i, boneIndex);
    }
}

void FbxAnimation::preloadKeyFrames(int boneIndex) {
    unsigned targetBone = 0;
    auto& boneData = mBoneParser.getBoneData();
    for (const auto& b : boneData) {
        //second(値)の方にボーンの番号が入っているため
        if (b.second.boneIndex == boneIndex) {
            targetBone = b.first;
        }
    }

    //ボーンと結びつくキーフレームノードを探す
    Vector3 tV, rV, sV;
    unsigned tNo = 0, rNo = 0, sNo = 0;
    for (const auto& c : mConnections) {
        if (c.parent != targetBone) {
            continue;
        }
        auto itr = mAnimationCurveNode.find(c.child);
        if (itr == mAnimationCurveNode.end()) {
            continue;
        }

        const auto& node = itr->second;
        auto type = node.type;
        if (type == TRS::T) {
            tV = node.value;
            tNo = c.child;
        } else if (type == TRS::R) {
            rV = node.value;
            rNo = c.child;
        } else if (type == TRS::S) {
            sV = node.value;
            sNo = c.child;
        }
    }

    //キーフレームの値を取得する
    auto& keyFrames = mKeyFrames[boneIndex];
    for (const auto& c : mConnections) {
        if (c.parent == tNo) {
            getKeyFrameData(keyFrames, tV, rV, sV, TRS::T, c.value, c.child);
        } else if (c.parent == rNo) {
            getKeyFrameData(keyFrames, tV, rV, sV, TRS::R, c.value, c.child);
        } else if (c.parent == sNo) {
            getKeyFrameData(keyFrames, tV, rV, sV, TRS::S, c.value, c.child);
        }
    }
}

void FbxAnimation::getKeyFrameData(
    KeyFrameData& out,
    const Vector3& t,
    const Vector3& r,
    const Vector3& s,
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

void FbxAnimation::calcKeyFrame(Matrix4& out, int frame, unsigned boneIndex) const {
    auto t = Vector3::zero;
    auto r = Vector3::zero;
    auto s = Vector3::one;
    calcKeyFrameValues(t, r, s, mKeyFrames[boneIndex], frame);

    out = Matrix4::createScale(s) * Matrix4::createFromQuaternion(Quaternion(r)) * Matrix4::createTranslation(t);
}

void FbxAnimation::calcKeyFrameValues(
    Vector3& t,
    Vector3& r,
    Vector3& s,
    const KeyFrameData& data,
    int frame
) const {
    //指定フレームでの時間を取得する
    const auto& time = mAnimationTime.getTime(frame);

    calcKeyFrameValue(t.x, data, frame, time, 0, 0); //tx
    calcKeyFrameValue(t.y, data, frame, time, 0, 1); //ty
    calcKeyFrameValue(t.z, data, frame, time, 0, 2); //tz
    calcKeyFrameValue(r.x, data, frame, time, 1, 0); //rx
    calcKeyFrameValue(r.y, data, frame, time, 1, 1); //ry
    calcKeyFrameValue(r.z, data, frame, time, 1, 2); //rz
    calcKeyFrameValue(s.x, data, frame, time, 2, 0); //sx
    calcKeyFrameValue(s.y, data, frame, time, 2, 1); //sy
    calcKeyFrameValue(s.z, data, frame, time, 2, 2); //sz
}

void FbxAnimation::calcKeyFrameValue(
    float& out,
    const KeyFrameData& data,
    int frame,
    long long time,
    int trs,
    int xyz
) const {
    const auto& keyValues = data.values;
    const auto& keyTimes = data.times;

    const auto& values = keyValues[trs][xyz];
    const auto& times = keyTimes[trs][xyz];
    //値が1つしかないならそれを使う
    if (values.size() == 1) {
        out = values[0];
        return;
    }
    if (frame < values.size()) {
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
