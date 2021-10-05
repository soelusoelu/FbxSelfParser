#include "FbxToDirectXMeshConverter.h"
#include "../FbxBone.h"
#include "../FbxMesh.h"
#include "../FbxWeight.h"

FbxToDirectXMeshConverter::FbxToDirectXMeshConverter(
    const FbxMesh& meshParser,
    const FbxBone& boneParser,
    const std::unordered_multimap<unsigned, unsigned>& connections
)
    : mMeshParser(meshParser)
    , mBoneParser(boneParser)
    , mConnections(connections)
{
}

FbxToDirectXMeshConverter::~FbxToDirectXMeshConverter() = default;

void FbxToDirectXMeshConverter::convert(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices
) const {
    int meshCount = mMeshParser.getMeshCount();
    meshesVertices.resize(meshCount);
    meshesIndices.resize(meshCount);

    for (int meshIdx = 0; meshIdx < meshCount; ++meshIdx) {
        auto& dstVertices = meshesVertices[meshIdx];
        auto& dstIndices = meshesIndices[meshIdx];

        if (mBoneParser.hasBone()) {
            //ボーンがある場合、位置、法線、UV + ウェイトを登録する
            positionNormalUVAndWeight(dstVertices, dstIndices, meshIdx);
        } else {
            //ボーンがない場合、位置、法線、UVのみ登録する
            positionNormalUV(dstVertices, dstIndices, meshIdx);
        }
    }
}

void FbxToDirectXMeshConverter::positionNormalUV(
    MeshVertices& meshVertices,
    Indices& meshIndices,
    unsigned meshIndex
) const {
    const auto& srcVertices = mMeshParser.getVertices(meshIndex);
    const auto& srcIndices = mMeshParser.getIndices(meshIndex);
    const auto& srcNormals = mMeshParser.getNormals(meshIndex);
    const auto& srcUVs = mMeshParser.getUVs(meshIndex);
    const auto& srcUVIndices = mMeshParser.getUVIndices(meshIndex);

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

void FbxToDirectXMeshConverter::positionNormalUVAndWeight(
    MeshVertices& meshVertices,
    Indices& meshIndices,
    unsigned meshIndex
) const {
    const auto& srcVertices = mMeshParser.getVertices(meshIndex);
    const auto& srcIndices = mMeshParser.getIndices(meshIndex);
    const auto& srcNormals = mMeshParser.getNormals(meshIndex);
    const auto& srcUVs = mMeshParser.getUVs(meshIndex);
    const auto& srcUVIndices = mMeshParser.getUVIndices(meshIndex);

    //面法線に合わせるために
    auto size = srcNormals.size();
    meshVertices.resize(size);
    meshIndices.resize(size);

    const auto& boneData = mBoneParser.getBoneData();
    const auto& weightParser = mBoneParser.getWeightParser();
    for (const auto& data : boneData) {
        //ボーンのノード番号と一致しているものをすべて取得する
        auto range = mConnections.equal_range(data.first);
        for (auto& itr = range.first; itr != range.second; ++itr) {
            //コネクションの右側がクラスターのノード番号と一致するか
            auto nodeIndex = itr->second;
            if (!weightParser.hasCluster(meshIndex, nodeIndex)) {
                continue;
            }

            //ここまできたらボーンとクラスターが一致している
            const auto& srcCluster = weightParser.getCluster(meshIndex, nodeIndex);
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

void FbxToDirectXMeshConverter::addWeight(
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

void FbxToDirectXMeshConverter::normalizeWeight(
    MeshVertices& meshVertice
) const {
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
