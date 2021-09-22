#include "FbxParser.h"
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
    , mMeshObject(nullptr)
    , mMaterialObject(nullptr)
    , mBoneObject(nullptr)
{
}

FbxParser::~FbxParser() = default;

void FbxParser::parse(const std::string& filePath) {
    FbxStream stream(filePath.c_str());
    mReader->parse(stream, *mRootObject);

    const auto& objects = getObject("Objects");
    mMeshObject = std::make_unique<FbxMesh>(objects);
    mMaterialObject = std::make_unique<FbxMaterial>(objects);
    mBoneObject = std::make_unique<FbxBone>(objects, getObject("Connections"));
}

const FbxObject& FbxParser::getRootObject() const {
    return *mRootObject;
}

const FbxObject& FbxParser::getObject(const std::string& name) const {
    return mRootObject->getObject(name);
}

const FbxMesh& FbxParser::getMeshParser() const {
    return *mMeshObject;
}

const FbxMaterial& FbxParser::getMaterialParser() const {
    return *mMaterialObject;
}

FbxBone& FbxParser::getBoneParser() const {
    return *mBoneObject;
}
