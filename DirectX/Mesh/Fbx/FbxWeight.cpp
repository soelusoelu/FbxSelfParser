#include "FbxWeight.h"
#include "FbxMesh.h"

FbxWeight::FbxWeight(const FbxObject& objectsObject)
    : mObjectsObject(objectsObject)
{
}

FbxWeight::~FbxWeight() = default;

void FbxWeight::parse(
    std::vector<MeshVertices>& meshesVertices,
    const std::vector<Indices>& meshesIndices,
    const FbxMesh& mesh
) {
    for (size_t i = 0; i < meshesVertices.size(); i++) {
        auto& meshVertices = meshesVertices[i];
        const auto& meshIndices = meshesIndices[i];

        unsigned boneIndex = 0;
        const auto& children = mObjectsObject.children;
        auto range = children.equal_range("Deformer");
        for (auto& itr = range.first; itr != range.second; ++itr) {
            const auto& obj = itr->second;
            if (obj.attributes[2] != "Cluster") {
                continue;
            }

            parseWeight(meshVertices, meshIndices, mesh, obj, boneIndex);
            ++boneIndex;
        }

        //ウェイト正規化
        if (boneIndex > 0) {
            normalizeWeight(meshVertices);
        }
    }
}

void FbxWeight::parseWeight(
    MeshVertices& meshVertices,
    const Indices& indices,
    const FbxMesh& mesh,
    const FbxObject& deformerObject,
    unsigned boneIndex
) {
    //重み
    const auto& weights = deformerObject.getArray("Weights");
    //影響する頂点の数
    unsigned weightCount = weights.size();
    //このボーンによって移動する頂点のインデックスの配列
    const auto& weightIndices = deformerObject.getArray("Indexes");
    //頂点のインデックス
    const auto& meshIndices = mesh.getIndices();

    for (unsigned i = 0; i < weightCount; ++i) {
        int index = std::stoi(weightIndices[i]);
        float weight = std::stof(weights[i]);

        for (int j = 0; j < meshVertices.size(); ++j) {
            //頂点番号と一致するのを探す
            if (meshIndices[j] == index) {
                //頂点にウェイト情報を追加する
                addWeight(meshVertices[indices[j]], weight, boneIndex);
            }
        }
    }
}

void FbxWeight::addWeight(MeshVertex& vertex, float weight, int boneIndex) {
    int insertPos = 0;
    //ウェイトの格納位置を探す
    for (int i = 0; i < MAX_INFLUENCE; ++i) {
        if (weight > vertex.weight[i]) {
            insertPos = i;
            break;
        }
    }
    //ウェイトが大きい順に格納していく
    //追加ウェイトより小さいウェイトを一つ右にずらしていく
    for (int i = insertPos; i < MAX_INFLUENCE; ++i) {
        auto tempW = vertex.weight[i];
        auto tempI = vertex.index[i];
        vertex.weight[i] = weight;
        vertex.index[i] = boneIndex;
        weight = tempW;
        boneIndex = tempI;
    }
}

void FbxWeight::normalizeWeight(MeshVertices& meshVertice) {
    //5本以上にまたっがてる場合のため
    for (size_t i = 0; i < meshVertice.size(); i++) {
        auto& vertex = meshVertice[i];
        float sumWeight = 0.f;
        //頂点のウェイトの合計を求める
        for (int j = 0; j < MAX_INFLUENCE; ++j) {
            float weight = vertex.weight[j];
            if (weight <= 0.f) {
                break;
            }
            sumWeight += weight;
        }

        //ボーンから影響を受けないなら次へ
        if (sumWeight <= 0.f) {
            continue;
        }

        //正規化
        for (int j = 0; j < MAX_INFLUENCE; ++j) {
            vertex.weight[j] /= sumWeight;
        }
    }
}
