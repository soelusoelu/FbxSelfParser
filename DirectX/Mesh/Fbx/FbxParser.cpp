#include "FbxParser.h"
#include "FbxAnimation.h"
#include "FbxBone.h"
#include "FbxMaterial.h"
#include "FbxMesh.h"
#include "Converter/FbxToDirectXConverter.h"
#include "Reader/FbxObject.h"
#include "Reader/FbxReader.h"
#include "Reader/FbxStream.h"
#include "../OriginalFormat/OriginalFormatWriter.h"

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
    std::vector<int>& materialIDs,
    std::vector<Bone>& bones,
    std::vector<Motion>& motions
) {
    FbxStream stream(filePath.c_str());
    mReader->parse(stream, *mRootObject);

    const auto& globalSettings = getObject("GlobalSettings");
    const auto& objects = getObject("Objects");
    const auto& connections = getObject("Connections").connections;
    for (const auto& c : connections) {
        mConnectionsMultimap.emplace(c.child, c.parent);
    }

    mMeshParser = std::make_unique<FbxMesh>(globalSettings, objects, mConnectionsMultimap);
    mMaterialParser = std::make_unique<FbxMaterial>(filePath, objects, mConnectionsMultimap, mMeshParser->getLclModelNodeIDs());
    mBoneParser = std::make_unique<FbxBone>(objects, *mMeshParser, mConnectionsMultimap);
    mAnimationParser = std::make_unique<FbxAnimation>(
        globalSettings,
        objects,
        *mBoneParser,
        mConnectionsMultimap,
        connections
    );

    auto converter = std::make_unique<FbxToDirectXConverter>(
        *mMeshParser,
        *mMaterialParser,
        *mBoneParser,
        *mAnimationParser,
        mConnectionsMultimap
    );
    converter->convertVerticesAndIndices(meshesVertices, meshesIndices);
    auto meshCount = meshesVertices.size();
    materialIDs.resize(meshCount);
    converter->convertMaterials(materialIDs);
    converter->convertBoneAnimation(bones, motions);

    OriginalFormatWriter originalWriter;
    originalWriter.writeFbxToOriginal(
        filePath,
        meshesVertices,
        meshesIndices,
        materialIDs,
        bones,
        motions
    );
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

const FbxMaterial& FbxParser::getMaterialParser() const {
    return *mMaterialParser;
}

const FbxBone& FbxParser::getBoneParser() const {
    return *mBoneParser;
}

const FbxAnimation& FbxParser::getAnimationParser() const {
    return *mAnimationParser;
}
