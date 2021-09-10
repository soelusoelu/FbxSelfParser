#include "SkinMeshComponent.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include <cassert>

SkinMeshComponent::SkinMeshComponent()
    : Component()
    , mAnimation(nullptr)
    , mMeshShader(nullptr)
    , mCurrentMotionNo(0)
    , mCurrentFrame(0)
    , mIsMotionUpdate(true)
    , mIsLoop(true)
{
}

SkinMeshComponent::~SkinMeshComponent() = default;

void SkinMeshComponent::update() {
    if (mIsMotionUpdate) {
        calcNextPose();
    }

    //gpu側でスキニングする場合に送る必要がある
    //mMeshShader->setTransferData(mCurrentBones.data(), sizeof(SkinMeshConstantBuffer), 3);
}

void SkinMeshComponent::setMotionName(const std::string& name, unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mAnimation->setMotionName(name, motionNo);
}

void SkinMeshComponent::changeMotion(unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mCurrentMotionNo = motionNo;
    mCurrentFrame = 0;
    mIsMotionUpdate = true;

    mCallbackChangeMotion();
}

void SkinMeshComponent::changeMotion(const std::string& motionName) {
    for (unsigned i = 0; i < mAnimation->getMotionCount(); ++i) {
        if (getMotion(i).name == motionName) {
            changeMotion(i);

            return;
        }
    }

    Debug::logError("Not found motion name[" + motionName + "].");
}

void SkinMeshComponent::tPose() {
    mIsMotionUpdate = false;
    mCurrentMotionNo = T_POSE_NO;
    mCurrentBones.assign(mCurrentBones.size(), Matrix4::identity);

    //通知を送る
    mCallbackComputeCurrentBones();
}

void SkinMeshComponent::setMotionUpdateFlag(bool value) {
    mIsMotionUpdate = value;
}

bool SkinMeshComponent::getMotionUpdateFlag() const {
    return mIsMotionUpdate;
}

void SkinMeshComponent::setLoop(bool value) {
    mIsLoop = value;
}

bool SkinMeshComponent::getLoop() const {
    return mIsLoop;
}

int SkinMeshComponent::getMotionCount() const {
    return mAnimation->getMotionCount();
}

const Motion& SkinMeshComponent::getMotion(unsigned motionNo) const {
    assert(motionNo < mAnimation->getMotionCount());
    return mAnimation->getMotion(motionNo);
}

const Motion& SkinMeshComponent::getMotion(const std::string& motionName) const {
    for (unsigned i = 0; i < mAnimation->getMotionCount(); ++i) {
        const auto& motion = getMotion(i);
        if (motion.name == motionName) {
            return motion;
        }
    }

    Debug::logError("Not found motion name[" + motionName + "].");
    return getCurrentMotion();
}

const Motion& SkinMeshComponent::getCurrentMotion() const {
    if (mCurrentMotionNo == T_POSE_NO) {
        return getMotion(0);
    }
    return getMotion(mCurrentMotionNo);
}

int SkinMeshComponent::getCurrentMotionNumber() const {
    return mCurrentMotionNo;
}

int SkinMeshComponent::getCurrentMotionFrame() const {
    return mCurrentFrame;
}

const std::vector<Matrix4>& SkinMeshComponent::getBoneCurrentFrameMatrix() const {
    return mCurrentBones;
}

void SkinMeshComponent::setValue(const std::shared_ptr<MeshShader>& meshShader, IAnimation* anim) {
    mAnimation = anim;
    mMeshShader = meshShader;

    mCurrentBones.resize(mAnimation->getBoneCount());
}

void SkinMeshComponent::callbackChangeMotion(const std::function<void()>& callback) {
    mCallbackChangeMotion += callback;
}

void SkinMeshComponent::callbackComputeCurrentBones(const std::function<void()>& callback) {
    mCallbackComputeCurrentBones += callback;
}

void SkinMeshComponent::calcNextPose() {
    const auto& motion = getCurrentMotion();

    //シェーダーにボーンのデータを渡す
    unsigned boneCount = mAnimation->getBoneCount();
    for (unsigned i = 0; i < boneCount; ++i) {
        mCurrentBones[i] = mAnimation->getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    }

    calcCurrentFrame();

    //通知を送る
    mCallbackComputeCurrentBones();
}

void SkinMeshComponent::calcCurrentFrame() {
    const auto& motion = getCurrentMotion();

    ++mCurrentFrame;

    if (mCurrentFrame < motion.numFrame) {
        return;
    }

    if (mIsLoop) {
        mCurrentFrame = 0;
    } else {
        mCurrentFrame = motion.numFrame - 1;
        mIsMotionUpdate = false;
    }
}
