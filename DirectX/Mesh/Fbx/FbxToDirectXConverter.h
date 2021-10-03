#pragma once

#include "FbxAnimationTime.h"
#include "../IMeshLoader.h"
#include "../Bone.h"
#include "../Motion.h"
#include <unordered_map>
#include <vector>

struct KeyFrameData;
class FbxMesh;
class FbxBone;
class FbxAnimation;

class FbxToDirectXConverter {
public:
    FbxToDirectXConverter(
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxToDirectXConverter();
    FbxToDirectXConverter(const FbxToDirectXConverter&) = delete;
    FbxToDirectXConverter& operator=(const FbxToDirectXConverter&) = delete;

    //頂点バッファ、インデックスバッファをDirectXで求められる形に変換する
    void convertVerticesAndIndices(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices,
        const FbxMesh& meshParser,
        const FbxBone& boneParser
    ) const;

    //アニメーション・ボーンをDirectXで求められる形に変換する
    void convertBoneAnimation(
        std::vector<Bone>& bones,
        std::vector<Motion>& motions,
        const FbxBone& boneParser,
        const FbxAnimation& animationParser
    ) const;

private:
    //頂点に位置、法線、UVを格納する
    void positionNormalUV(
        MeshVertices& meshVertices,
        Indices& meshIndices,
        const FbxMesh& meshParser,
        unsigned meshIndex
    ) const;
    //頂点に位置、法線、UV + ウェイトを格納する
    void positionNormalUVAndWeight(
        MeshVertices& meshVertices,
        Indices& meshIndices,
        const FbxMesh& meshParser,
        const FbxBone& boneParser,
        unsigned meshIndex
    ) const;
    //新しいウェイトを格納する
    void addWeight(
        MeshVertex& vertex,
        float weight,
        int boneIndex
    ) const;
    //頂点ウェイトを正規化する
    void normalizeWeight(MeshVertices& meshVertice) const;
    //ボーンをDirectXで求められる形に変換する
    void convertBone(
        std::vector<Bone>& bones,
        const FbxBone& boneParser
    ) const;
    //アニメーションをDirectXで求められる形に変換する
    void convertAnimation(
        std::vector<Motion>& motions,
        const FbxAnimation& animationParser,
        unsigned boneCount
    ) const;
    //全キーフレームを読み込む
    void loadAllKeyFrames(
        Motion& motion,
        const FbxAnimation& animationParser,
        const FbxAnimationTime& animationTime,
        unsigned boneCount
    ) const;
    //キーフレーム読み込み
    void loadKeyFrames(
        Motion& motion,
        const KeyFrameData& keyFrames,
        const FbxAnimationTime& animationTime,
        unsigned boneIndex
    ) const;
    //キーフレームを求める
    void calcKeyFrame(
        Matrix4& out,
        const KeyFrameData& keyFrames,
        long long time,
        int frame
    ) const;
    void calcKeyFrameValues(
        Vector3& t,
        Vector3& r,
        Vector3& s,
        const KeyFrameData& keyFrames,
        long long time,
        int frame
    ) const;
    void calcKeyFrameValue(
        float& out,
        const KeyFrameData& keyFrames,
        int frame,
        long long time,
        int trs,
        int xyz
    ) const;

private:
    const std::unordered_multimap<unsigned, unsigned>& mConnections;
};
