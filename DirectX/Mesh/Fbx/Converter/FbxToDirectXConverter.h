#pragma once

#include "../../IMeshLoader.h"
#include "../../Bone.h"
#include "../../Motion.h"
#include <memory>
#include <unordered_map>
#include <vector>

class FbxMesh;
class FbxMaterial;
class FbxBone;
class FbxAnimation;
class FbxToDirectXMeshConverter;
class FbxToDirectXMaterialConverter;
class FbxToDirectXAnimationConverter;

class FbxToDirectXConverter {
public:
    FbxToDirectXConverter(
        const FbxMesh& meshParser,
        const FbxMaterial& materialParser,
        const FbxBone& boneParser,
        const FbxAnimation& animationParser,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxToDirectXConverter();
    FbxToDirectXConverter(const FbxToDirectXConverter&) = delete;
    FbxToDirectXConverter& operator=(const FbxToDirectXConverter&) = delete;

    //頂点バッファ、インデックスバッファをDirectXで求められる形に変換する
    void convertVerticesAndIndices(
        std::vector<MeshVertices>& meshesVertices,
        std::vector<Indices>& meshesIndices
    ) const;

    //マテリアルをDirectXで求められる形に変換する
    void convertMaterials(
        std::vector<int>& materialIDs
    ) const;

    //アニメーション・ボーンをDirectXで求められる形に変換する
    void convertBoneAnimation(
        std::vector<Bone>& bones,
        std::vector<Motion>& motions
    ) const;

private:
    std::unique_ptr<FbxToDirectXMeshConverter> mMeshConverter;
    std::unique_ptr<FbxToDirectXMaterialConverter> mMaterialConverter;
    std::unique_ptr<FbxToDirectXAnimationConverter> mAnimationConverter;
};
