#include "AnimationCPU.h"
#include "MeshComponent.h"
#include "SkinMeshComponent.h"
#include "../../../System/SystemInclude.h"
#include <algorithm>
#include <cassert>

AnimationCPU::AnimationCPU()
    : Component()
    , mMesh(nullptr)
    , mAnimation(nullptr)
{
}

AnimationCPU::~AnimationCPU() = default;

void AnimationCPU::start() {
    auto meshComp = getComponent<MeshComponent>();
    mMesh = meshComp->getMesh();

    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackComputeCurrentBones([&] { updateVertexPositionsAfterSkinning(); });

    auto meshCount = mMesh->getMeshCount();
    mCurrentMeshesVertexPositions.resize(meshCount);
    mCurrentMeshesVertices.resize(meshCount);
    for (size_t i = 0; i < meshCount; ++i) {
        const auto& meshVertices = mMesh->getMeshVertices(i);
        mCurrentMeshesVertexPositions[i].resize(meshVertices.size());
        mCurrentMeshesVertices[i].resize(meshVertices.size());
        std::copy(meshVertices.begin(), meshVertices.end(), mCurrentMeshesVertices[i].begin());
    }
}

const MeshVerticesPosition& AnimationCPU::getCurrentMotionVertexPositions(unsigned index) const {
    assert(index < mMesh->getMeshCount());
    return mCurrentMeshesVertexPositions[index];
}

void AnimationCPU::updateVertexPositionsAfterSkinning() {
    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        auto& vertexPositions = mCurrentMeshesVertexPositions[i];
        auto& meshVertices = mCurrentMeshesVertices[i];
        const auto& vertices = mMesh->getMeshVertices(i);
        for (size_t j = 0; j < vertices.size(); ++j) {
            Matrix4 comb{};
            ZeroMemory(&comb, sizeof(comb));
            computeCombinationMatrix(comb, vertices[j]);

            auto result = Vector3::transform(vertices[j].pos, comb);
            vertexPositions[j] = result;
            meshVertices[j].pos = result;
        }

        //スキニング後の新しい頂点情報を設定する
        mMesh->setMeshVertices(meshVertices, i);
    }
}

void AnimationCPU::computeCombinationMatrix(Matrix4& out, const MeshVertex& vertex) {
    for (int i = 0; i < 4; ++i) {
        const auto& currentBones = mAnimation->getBoneCurrentFrameMatrix();
        const auto& bone = currentBones[vertex.index[i]];
        float weigth = vertex.weight[i];
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                out.m[j][k] += bone.m[j][k] * weigth;
            }
        }
    }
}
