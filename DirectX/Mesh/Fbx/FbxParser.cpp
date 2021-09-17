#include "FbxParser.h"
#include "FbxMesh.h"
#include "Reader/FbxObject.h"
#include "Reader/FbxReader.h"
#include "Reader/FbxStream.h"
#include <cassert>

FbxParser::FbxParser()
    : mReader(std::make_unique<FbxReader>())
    , mRootObject(std::make_unique<FbxObject>())
    , mMeshObject(nullptr)
{
}

FbxParser::~FbxParser() = default;

void FbxParser::parse(const std::string& filePath) {
    FbxStream stream(filePath.c_str());
    mReader->parse(stream, *mRootObject);

    const auto& objects = getObject("Objects");
    const auto& geometry = objects.getObject("Geometry");
    mMeshObject = std::make_unique<FbxMesh>(geometry);
}

const FbxObject& FbxParser::getRootObject() const {
    return *mRootObject;
}

const FbxObject& FbxParser::getObject(const std::string& name) const {
    return mRootObject->getObject(name);
}

const FbxMesh& FbxParser::getMesh() const {
    return *mMeshObject;
}
