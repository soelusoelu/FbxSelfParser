#pragma once

#include "Reader/FbxObject.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class FbxMesh;

struct Cluster {
    std::vector<unsigned short> indexes;
    std::vector<float> weights;
};

class FbxWeight {
public:
    FbxWeight(
        const FbxObject& objectsObject,
        const FbxMesh& meshParser,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxWeight();
    FbxWeight(const FbxWeight&) = delete;
    FbxWeight& operator=(const FbxWeight&) = delete;

    const Cluster& getCluster(unsigned meshIndex, unsigned nodeIndex) const;
    bool hasCluster(unsigned meshIndex, unsigned nodeIndex) const;

private:
    void parseSkin(const FbxObject& objectsObject);

    void parseIndexesAndWeights(
        const FbxObject& objectsObject,
        const FbxMesh& meshParser,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );

private:
    //DeformerのSkinノード番号を保持する
    std::unordered_set<unsigned> mSkinSet;
    //vector: メッシュ順
    //map key: クラスターノード番号, value: クラスター
    std::vector<std::unordered_map<unsigned, Cluster>> mClusters;

    static constexpr int MAX_INFLUENCE = 4;
};
