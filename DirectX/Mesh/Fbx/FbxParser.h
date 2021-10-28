#pragma once

#include "Reader/FbxObject.h"
#include "../IMeshLoader.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class FbxReader;
class FbxMesh;
class FbxMaterial;
class FbxBone;
class FbxAnimation;

class FbxParser {
public:
    FbxParser();
    ~FbxParser();
    FbxParser(const FbxParser&) = delete;
    FbxParser& operator=(const FbxParser&) = delete;

    void parse(
        const std::string& filePath,
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices,
        std::vector<int>& materialIDs,
        std::vector<Bone>& bones,
        std::vector<Motion>& motions
    );

    const FbxObject& getRootObject() const;
    const FbxObject& getObject(const std::string& name) const;
    const FbxMesh& getMeshParser() const;
    const FbxMaterial& getMaterialParser() const;
    const FbxBone& getBoneParser() const;
    const FbxAnimation& getAnimationParser() const;

private:
    std::unique_ptr<FbxReader> mReader;
    std::unique_ptr<FbxObject> mRootObject;
    std::unique_ptr<FbxMesh> mMeshParser;
    std::unique_ptr<FbxMaterial> mMaterialParser;
    std::unique_ptr<FbxBone> mBoneParser;
    std::unique_ptr<FbxAnimation> mAnimationParser;

    std::unordered_multimap<unsigned, unsigned> mConnectionsMultimap;
};
