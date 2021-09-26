#pragma once

#include "Reader/FbxObject.h"
#include "../Bone.h"
#include "../IMeshLoader.h"
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

class FbxMesh;
class FbxWeight;

class FbxBone {
public:
    FbxBone(
        const FbxObject& objectsObject,
        const std::multimap<unsigned, unsigned>& connections
    );
    ~FbxBone();
    FbxBone(const FbxBone&) = delete;
    FbxBone& operator=(const FbxBone&) = delete;

    void parse(
        std::vector<Bone>& bones,
        std::vector<MeshVertices>& meshesVertices,
        const std::vector<Indices>& meshesIndices,
        const FbxMesh& mesh
    );

private:
    void parseLimbNode(std::vector<Bone>& bones);
    void parseBone(std::vector<Bone>& bones, const FbxObject& poseObject) const;
    void connect(std::vector<Bone>& bones) const;

private:
    const FbxObject& mObjectsObject;
    const std::multimap<unsigned, unsigned>& mConnections;
    std::unique_ptr<FbxWeight> mWeightParser;
    std::unordered_map<unsigned, unsigned short> mBoneConnections;
};
