#pragma once

#include "Reader/FbxObject.h"
#include "../Bone.h"
#include "../IMeshLoader.h"
#include <memory>
#include <vector>
#include <unordered_map>

class FbxMesh;
class FbxWeight;

class FbxBone {
public:
    FbxBone(const FbxObject& objectsObject, const FbxObject& connectionsObject);
    ~FbxBone();
    FbxBone(const FbxBone&) = delete;
    FbxBone& operator=(const FbxBone&) = delete;

    void parse(std::vector<Bone>& bones, MeshVertices& meshVertices, const Indices& indices, const FbxMesh& mesh);

private:
    void parseLimbNode(std::vector<Bone>& bones);
    void parseBone(std::vector<Bone>& bones, const FbxObject& poseObject) const;
    void connect(std::vector<Bone>& bones) const;

private:
    const FbxObject& mObjectsObject;
    const FbxObject& mConnectionsObject;
    std::unique_ptr<FbxWeight> mWeightParser;
    std::unordered_map<unsigned, unsigned short> mConnections;
};
