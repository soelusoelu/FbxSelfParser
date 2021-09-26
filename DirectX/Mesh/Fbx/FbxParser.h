#pragma once

#include "Reader/FbxObject.h"
#include <memory>
#include <string>

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

    void parse(const std::string& filePath);

    const FbxObject& getRootObject() const;
    const FbxObject& getObject(const std::string& name) const;
    const FbxMesh& getMeshParser() const;
    FbxMaterial& getMaterialParser() const;
    FbxBone& getBoneParser() const;
    FbxAnimation& getAnimationParser() const;

private:
    std::unique_ptr<FbxReader> mReader;
    std::unique_ptr<FbxObject> mRootObject;
    std::unique_ptr<FbxMesh> mMeshParser;
    std::unique_ptr<FbxMaterial> mMaterialParser;
    std::unique_ptr<FbxBone> mBoneParser;
    std::unique_ptr<FbxAnimation> mAnimationParser;
};
