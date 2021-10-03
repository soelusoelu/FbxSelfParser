#include "FbxToDirectXConverter.h"
#include "FbxAnimation.h"
#include "FbxBone.h"
#include "FbxMesh.h"
#include "FbxWeight.h"
#include <cassert>

FbxToDirectXConverter::FbxToDirectXConverter(
    const std::unordered_multimap<unsigned, unsigned>& connections
)
    : mConnections(connections)
{
}

FbxToDirectXConverter::~FbxToDirectXConverter() = default;

void FbxToDirectXConverter::convertVerticesAndIndices(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    const FbxMesh& meshParser,
    const FbxBone& boneParser
) const {
    int meshCount = meshParser.getMeshCount();
    meshesVertices.resize(meshCount);
    meshesIndices.resize(meshCount);

    for (int meshIdx = 0; meshIdx < meshCount; ++meshIdx) {
        auto& dstVertices = meshesVertices[meshIdx];
        auto& dstIndices = meshesIndices[meshIdx];

        if (boneParser.hasBone()) {
            //ボーンがある場合、位置、法線、UV + ウェイトを登録する
            positionNormalUVAndWeight(dstVertices, dstIndices, meshParser, boneParser, meshIdx);
        } else {
            //ボーンがない場合、位置、法線、UVのみ登録する
            positionNormalUV(dstVertices, dstIndices, meshParser, meshIdx);
        }
    }
}

void FbxToDirectXConverter::convertBoneAnimation(
    std::vector<Bone>& bones,
    std::vector<Motion>& motions,
    const FbxBone& boneParser,
    const FbxAnimation& animationParser
) const {
    //ボーン数
    unsigned boneCount = boneParser.getBoneCount();

    //ボーンがないなら終了
    if (boneCount == 0) {
        return;
    }

    //ボーンを変換する
    convertBone(bones, boneParser);

    //アニメーションを変換する
    convertAnimation(motions, animationParser, boneCount);
}

void FbxToDirectXConverter::positionNormalUV(
    MeshVertices& meshVertices,
    Indices& meshIndices,
    const FbxMesh& meshParser,
    unsigned meshIndex
) const {
    const auto& srcVertices = meshParser.getVertices(meshIndex);
    const auto& srcIndices = meshParser.getIndices(meshIndex);
    const auto& srcNormals = meshParser.getNormals(meshIndex);
    const auto& srcUVs = meshParser.getUVs(meshIndex);
    const auto& srcUVIndices = meshParser.getUVIndices(meshIndex);

    //面法線に合わせるために
    auto size = srcNormals.size();
    meshVertices.resize(size);
    meshIndices.resize(size);

    for (size_t i = 0; i < size; ++i) {
        auto& v = meshVertices[i];
        meshIndices[i] = static_cast<unsigned short>(i);
        v.pos = srcVertices[srcIndices[i]];
        v.normal = srcNormals[i];
        v.uv = srcUVs[srcUVIndices[i]];
    }
}

void FbxToDirectXConverter::positionNormalUVAndWeight(
    MeshVertices& meshVertices,
    Indices& meshIndices,
    const FbxMesh& meshParser,
    const FbxBone& boneParser,
    unsigned meshIndex
) const {
    const auto& srcVertices = meshParser.getVertices(meshIndex);
    const auto& srcIndices = meshParser.getIndices(meshIndex);
    const auto& srcNormals = meshParser.getNormals(meshIndex);
    const auto& srcUVs = meshParser.getUVs(meshIndex);
    const auto& srcUVIndices = meshParser.getUVIndices(meshIndex);

    //面法線に合わせるために
    auto size = srcNormals.size();
    meshVertices.resize(size);
    meshIndices.resize(size);

    const auto& boneData = boneParser.getBoneData();
    const auto& weightParser = boneParser.getWeightParser();
    for (const auto& data : boneData) {
        //ボーンのノード番号と一致しているものをすべて取得する
        auto range = mConnections.equal_range(data.first);
        for (auto& itr = range.first; itr != range.second; ++itr) {
            //コネクションの右側がクラスターのノード番号と一致するか
            auto nodeIndex = itr->second;
            if (!weightParser.hasCluster(nodeIndex)) {
                continue;
            }

            //ここまできたらボーンとクラスターが一致している
            const auto& srcCluster = weightParser.getCluster(nodeIndex);
            const auto& weights = srcCluster.weights;
            const auto& indexes = srcCluster.indexes;
            auto weightCount = weights.size();

            //頂点配列に合わせてウェイト配列を作成する
            std::vector<float> weightArray(srcVertices.size(), 0.f);
            for (size_t i = 0; i < weightCount; ++i) {
                weightArray[indexes[i]] = weights[i];
            }

            for (size_t i = 0; i < size; ++i) {
                auto& v = meshVertices[i];
                meshIndices[i] = static_cast<unsigned short>(i);
                v.pos = srcVertices[srcIndices[i]];
                v.normal = srcNormals[i];
                v.uv = srcUVs[srcUVIndices[i]];

                //頂点にウェイト情報を追加する
                float weight = weightArray[srcIndices[i]];
                if (weight > 0.f) {
                    addWeight(v, weight, data.second.boneIndex);
                }
            }
        }
    }

    //ウェイト正規化
    normalizeWeight(meshVertices);
}

void FbxToDirectXConverter::addWeight(
    MeshVertex& vertex,
    float weight,
    int boneIndex
) const {
    int insertPos = 0;
    //ウェイトの格納位置を探す
    for (int i = 0; i < MAX_NUM_WEIGHT; ++i) {
        if (weight > vertex.weight[i]) {
            insertPos = i;
            break;
        }
    }
    //ウェイトが大きい順に格納していく
    //追加ウェイトより小さいウェイトを一つ右にずらしていく
    for (int i = insertPos; i < MAX_NUM_WEIGHT; ++i) {
        auto tempW = vertex.weight[i];
        auto tempI = vertex.index[i];
        vertex.weight[i] = weight;
        vertex.index[i] = boneIndex;
        weight = tempW;
        boneIndex = tempI;
    }
}

void FbxToDirectXConverter::normalizeWeight(MeshVertices& meshVertice) const {
    //5本以上にまたっがている場合にそなえて
    for (auto&& vertex : meshVertice) {
        float sumWeight = 0.f;
        //頂点のウェイトの合計を求める
        for (int i = 0; i < MAX_NUM_WEIGHT; ++i) {
            float weight = vertex.weight[i];
            if (weight <= 0.f) {
                break;
            }
            sumWeight += weight;
        }

        //正規化
        for (int i = 0; i < MAX_NUM_WEIGHT; ++i) {
            vertex.weight[i] /= sumWeight;
        }
    }
}

void FbxToDirectXConverter::convertBone(
    std::vector<Bone>& bones,
    const FbxBone& boneParser
) const {
    bones.resize(boneParser.getBoneCount());
    const auto& boneData = boneParser.getBoneData();
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

void FbxToDirectXConverter::convertAnimation(
    std::vector<Motion>& motions,
    const FbxAnimation& animationParser,
    unsigned boneCount
) const {
    auto animationCount = animationParser.getAnimationCount();
    motions.resize(animationCount);

    for (size_t i = 0; i < animationCount; ++i) {
        auto& motion = motions[i];

        const auto& stack = animationParser.getAnimationStack(i);
        //モーション名を設定する
        motion.name = stack.name;
        //モーションのフレーム数を設定
        const auto& time = stack.time;
        motion.numFrame = time.stopFrame - time.startFrame;

        //キーフレームの読み込み
        loadAllKeyFrames(motion, animationParser, time, boneCount);
    }
}

void FbxToDirectXConverter::loadAllKeyFrames(
    Motion& motion,
    const FbxAnimation& animationParser,
    const FbxAnimationTime& animationTime,
    unsigned boneCount
) const {
    //ボーン数分拡張しとく
    motion.frameMatrix.resize(boneCount);
    for (unsigned i = 0; i < boneCount; ++i) {
        //キーフレーム読み込み
        loadKeyFrames(motion, animationParser.getKeyFrameData(i), animationTime, i);
    }
}

void FbxToDirectXConverter::loadKeyFrames(
    Motion& motion,
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
        calcKeyFrame(m[i], keyFrames, time, i);
    }
}

void FbxToDirectXConverter::calcKeyFrame(
    Matrix4& out,
    const KeyFrameData& keyFrames,
    long long time,
    int frame
) const {
    auto t = Vector3::zero;
    auto r = Vector3::zero;
    auto s = Vector3::one;
    calcKeyFrameValues(t, r, s, keyFrames, time, frame);

    out = Matrix4::createScale(s) * Matrix4::createFromQuaternion(Quaternion(r)) * Matrix4::createTranslation(t);
}

void FbxToDirectXConverter::calcKeyFrameValues(
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

void FbxToDirectXConverter::calcKeyFrameValue(
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
