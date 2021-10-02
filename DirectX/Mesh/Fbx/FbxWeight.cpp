#include "FbxWeight.h"
#include "FbxMesh.h"
#include <cassert>

FbxWeight::FbxWeight(const FbxObject& objectsObject)
    : mObjectsObject(objectsObject)
{
    parseIndexesAndWeights();
}

FbxWeight::~FbxWeight() = default;

const Cluster& FbxWeight::getCluster(unsigned nodeIndex) const {
    return mClusters.at(nodeIndex);
}

bool FbxWeight::hasCluster(unsigned nodeIndex) const {
    return (mClusters.find(nodeIndex) != mClusters.end());
}

void FbxWeight::parseIndexesAndWeights() {
    auto deformers = mObjectsObject.children.equal_range("Deformer");
    for (auto& itr = deformers.first; itr != deformers.second; ++itr) {
        const auto& obj = itr->second;
        //3つ目のアトリビュートでボーンか判断
        if (obj.attributes[2] != "Cluster") {
            continue;
        }

        const auto& srcIndexes = obj.getArray("Indexes");
        const auto& srcWeights = obj.getArray("Weights");
        auto size = srcIndexes.size();
        assert(size == srcWeights.size());

        Cluster cluster;
        auto& indexes = cluster.indexes;
        auto& weights = cluster.weights;

        indexes.resize(size);
        weights.resize(size);

        for (size_t i = 0; i < size; ++i) {
            indexes[i] = std::stoi(srcIndexes[i]);
            weights[i] = std::stof(srcWeights[i]);
        }

        mClusters.emplace(obj.getNodeId(), cluster);
    }
}
