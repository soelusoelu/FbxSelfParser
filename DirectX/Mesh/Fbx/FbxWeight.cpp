#include "FbxWeight.h"
#include "FbxMesh.h"
#include <cassert>

FbxWeight::FbxWeight(
    const FbxObject& objectsObject,
    const FbxMesh& meshParser,
    const std::unordered_multimap<unsigned, unsigned>& connections
) {
    parseSkin(objectsObject);
    parseIndexesAndWeights(objectsObject, meshParser, connections);
}

FbxWeight::~FbxWeight() = default;

const Cluster& FbxWeight::getCluster(unsigned meshIndex, unsigned nodeIndex) const {
    assert(meshIndex < mClusters.size());
    return mClusters[meshIndex].at(nodeIndex);
}

bool FbxWeight::hasCluster(unsigned meshIndex, unsigned nodeIndex) const {
    assert(meshIndex < mClusters.size());
    const auto& clusters = mClusters[meshIndex];
    return (clusters.find(nodeIndex) != clusters.end());
}

void FbxWeight::parseSkin(const FbxObject& objectsObject) {
    auto deformers = objectsObject.children.equal_range("Deformer");
    for (auto& itr = deformers.first; itr != deformers.second; ++itr) {
        const auto& obj = itr->second;

        //3つ目のアトリビュートでスキンか判断
        if (obj.attributes[2] != "Skin") {
            continue;
        }

        //ノード番号を追加する
        mSkinSet.emplace(obj.getNodeId());
    }
}

void FbxWeight::parseIndexesAndWeights(
    const FbxObject& objectsObject,
    const FbxMesh& meshParser,
    const std::unordered_multimap<unsigned, unsigned>& connections
) {
    const auto& geometryMap = meshParser.geGeometryNodeIDs();
    mClusters.resize(geometryMap.size());

    auto deformers = objectsObject.children.equal_range("Deformer");
    for (auto& itr = deformers.first; itr != deformers.second; ++itr) {
        const auto& obj = itr->second;
        //3つ目のアトリビュートでボーンか判断
        if (obj.attributes[2] != "Cluster") {
            continue;
        }
        //Indexes配列がなければ次へ
        if (!obj.hasArray("Indexes")) {
            continue;
        }

        //IndexesとWeights配列を取得
        const auto& srcIndexes = obj.getArray("Indexes");
        const auto& srcWeights = obj.getArray("Weights");
        auto size = srcIndexes.size();
        assert(size == srcWeights.size());

        //クラスター構造体に追加する
        Cluster cluster;
        auto& indexes = cluster.indexes;
        auto& weights = cluster.weights;

        indexes.resize(size);
        weights.resize(size);

        for (size_t i = 0; i < size; ++i) {
            indexes[i] = std::stoi(srcIndexes[i]);
            weights[i] = std::stof(srcWeights[i]);
        }

        //対象となるSkinを探す
        auto nodeId = obj.getNodeId();
        unsigned targetSkin = 0;
        auto nodes = connections.equal_range(nodeId);
        for (auto& n = nodes.first; n != nodes.second; ++n) {
            if (mSkinSet.find(n->second) != mSkinSet.end()) {
                targetSkin = n->second;
                break;
            }
        }
        assert(targetSkin > 0);

        //対象となるGeometryを探す
        unsigned short geometryIndex = 0;
        nodes = connections.equal_range(targetSkin);
        for (auto& n = nodes.first; n != nodes.second; ++n) {
            auto f = geometryMap.find(n->second);
            if (f != geometryMap.end()) {
                geometryIndex = f->second;
                break;
            }
        }

        //格納
        mClusters[geometryIndex].emplace(obj.getNodeId(), cluster);
    }
}
