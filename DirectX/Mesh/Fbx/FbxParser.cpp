﻿#include "FbxParser.h"
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

void FbxParser::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials,
    std::vector<Bone>& bones,
    std::vector<Motion>& motions
) {
    FbxStream stream(filePath.c_str());
    mReader->parse(stream, *mRootObject);

    const auto& objects = getObject("Objects");
    const auto& connections = getObject("Connections").connections;
    for (const auto& c : connections) {
        mConnectionsMultiMap.emplace(c.child, c.parent);
    }

    mMeshParser = std::make_unique<FbxMesh>(objects, mConnectionsMultiMap);
    mMaterialParser = std::make_unique<FbxMaterial>(objects, mConnectionsMultiMap);
    mBoneParser = std::make_unique<FbxBone>(objects, mConnectionsMultiMap);
    mAnimationParser = std::make_unique<FbxAnimation>(getObject("GlobalSettings"), objects);

    mMeshParser->parse(meshesVertices, meshesIndices);
    auto meshCount = meshesVertices.size();
    materials.resize(meshCount);
    mMaterialParser->parse(materials, filePath, mMeshParser->getLclModelNodeIDs());
    //mBoneParser->parse(bones, meshesVertices, meshesIndices, *mMeshParser);
    //mAnimationParser->parse(motions, bones);
}

const FbxObject& FbxParser::getRootObject() const {
    return *mRootObject;
}

const FbxObject& FbxParser::getObject(const std::string& name) const {
    return mRootObject->getObject(name);
}
