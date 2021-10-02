#pragma once

#include "Reader/FbxObject.h"
#include <unordered_map>
#include <vector>

struct Cluster {
    std::vector<unsigned short> indexes;
    std::vector<float> weights;
};

class FbxWeight {
public:
    FbxWeight(const FbxObject& objectsObject);
    ~FbxWeight();
    FbxWeight(const FbxWeight&) = delete;
    FbxWeight& operator=(const FbxWeight&) = delete;

    const Cluster& getCluster(unsigned nodeIndex) const;
    bool hasCluster(unsigned nodeIndex) const;

private:
    void parseIndexesAndWeights();

private:
    const FbxObject& mObjectsObject;
    std::unordered_map<unsigned, Cluster> mClusters;

    static constexpr int MAX_INFLUENCE = 4;
};
