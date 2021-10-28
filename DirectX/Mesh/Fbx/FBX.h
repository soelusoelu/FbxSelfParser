#pragma once

#include "../IMeshLoader.h"
#include <string>

class FBX
    : public IMeshLoader
{
public:
    FBX();
    ~FBX();
    FBX(const FBX&) = delete;
    FBX& operator=(const FBX&) = delete;

    virtual void parse(
        const std::string& filePath,
        std::vector<MeshVertices>& meshesVertices,
        std::vector<MeshVerticesPosition>& meshesVerticesPosition,
        std::vector<Indices>& meshesIndices,
        std::vector<int>& materialIDs,
        std::vector<Motion>& motions,
        std::vector<Bone>& bones
    ) override;
};
