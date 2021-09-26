#include "FbxParser.h"
#include "FbxAnimation.h"
#include "FbxBone.h"
#include "FbxMaterial.h"
#include "FbxMesh.h"
#include "Reader/FbxObject.h"
#include "Reader/FbxReader.h"
#include "Reader/FbxStream.h"
#include <cassert>

FbxParser::FbxParser()
    : mReader(std::make_unique<FbxReader>())
    , mRootObject(std::make_unique<FbxObject>())
    , mMeshParser(nullptr)
    , mMaterialParser(nullptr)
    , mBoneParser(nullptr)
    , mAnimationParser(nullptr)
{
}

FbxParser::~FbxParser() = default;

void FbxParser::parse(const std::string& filePath) {
    FbxStream stream(filePath.c_str());
    mReader->parse(stream, *mRootObject);

    const auto& objects = getObject("Objects");
    const auto& connections = getObject("Connections");

    mMeshParser = std::make_unique<FbxMesh>(objects, connections);
    mMaterialParser = std::make_unique<FbxMaterial>(objects, connections);
    mBoneParser = std::make_unique<FbxBone>(objects, connections);
    mAnimationParser = std::make_unique<FbxAnimation>(getObject("GlobalSettings"), objects);
}

const FbxObject& FbxParser::getRootObject() const {
    return *mRootObject;
}

const FbxObject& FbxParser::getObject(const std::string& name) const {
    return mRootObject->getObject(name);
}

const FbxMesh& FbxParser::getMeshParser() const {
    return *mMeshParser;
}

FbxMaterial& FbxParser::getMaterialParser() const {
    return *mMaterialParser;
}

FbxBone& FbxParser::getBoneParser() const {
    return *mBoneParser;
}

FbxAnimation& FbxParser::getAnimationParser() const {
    return *mAnimationParser;
}
