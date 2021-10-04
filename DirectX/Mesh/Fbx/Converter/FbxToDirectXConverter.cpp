#include "FbxToDirectXConverter.h"
#include "FbxToDirectXAnimationConverter.h"
#include "FbxToDirectXMeshConverter.h"

FbxToDirectXConverter::FbxToDirectXConverter(
    const FbxMesh& meshParser,
    const FbxBone& boneParser,
    const FbxAnimation& animationParser,
    const std::unordered_multimap<unsigned, unsigned>& connections
)
    : mMeshConverter(std::make_unique<FbxToDirectXMeshConverter>(meshParser, boneParser, connections))
    , mAnimationConverter(std::make_unique<FbxToDirectXAnimationConverter>(boneParser, animationParser, connections))
{
}

FbxToDirectXConverter::~FbxToDirectXConverter() = default;

void FbxToDirectXConverter::convertVerticesAndIndices(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<Indices>& meshesIndices
) const {
    mMeshConverter->convert(meshesVertices, meshesIndices);
}

void FbxToDirectXConverter::convertBoneAnimation(
    std::vector<Bone>& bones,
    std::vector<Motion>& motions
) const {
    mAnimationConverter->convert(bones, motions);
}
