#pragma once

#include "../IMeshLoader.h"

class OriginalFormatReader
    : public IMeshLoader
{
public:
    OriginalFormatReader();
    ~OriginalFormatReader();
    OriginalFormatReader(const OriginalFormatReader&) = delete;
    OriginalFormatReader& operator=(const OriginalFormatReader&) = delete;

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
