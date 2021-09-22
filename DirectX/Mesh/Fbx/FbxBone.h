#pragma once

#include "Reader/FbxObject.h"
#include "../Bone.h"
#include <vector>
#include <unordered_map>

class FbxBone {
public:
    FbxBone(const FbxObject& objectsObject, const FbxObject& connectionsObject);
    ~FbxBone();
    FbxBone(const FbxBone&) = delete;
    FbxBone& operator=(const FbxBone&) = delete;

    void parse(std::vector<Bone>& bones);

private:
    void parseBone(std::vector<Bone>& bones, const FbxObject& poseObject);
    void connect(std::vector<Bone>& bones);

private:
    const FbxObject& mObjectsObject;
    const FbxObject& mConnectionsObject;
    std::unordered_map<unsigned, unsigned> mConnections;
};
