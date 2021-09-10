#include "MeshRenderOnTextureComponent.h"
#include "../Camera/CameraHelper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Sprite/Sprite.h"
#include "../../../System/Texture/MeshRenderOnTexture.h"
#include "../../../System/Texture/MeshRenderOnTextureManager.h"
#include "../../../Utility/JsonHelper.h"

MeshRenderOnTextureComponent::MeshRenderOnTextureComponent()
    : Component()
    , mMeshRenderOnTexture(std::make_unique<MeshRenderOnTexture>())
    , mSphereCoverMesh()
    , mView()
    , mProjection()
{
}

MeshRenderOnTextureComponent::~MeshRenderOnTextureComponent() = default;

void MeshRenderOnTextureComponent::start() {
    if (!mManager) {
        Debug::logError("MeshRenderOnTextureManager is null.");
        return;
    }

    //射影行列を作成する
    mProjection = Matrix4::createPerspectiveFOV(1.f, 45.f, 0.1f, 1000.f);

    mManager->add(shared_from_this());
}

void MeshRenderOnTextureComponent::update() {
    mMeshRenderOnTexture->getSprite().computeWorldTransform();
}

void MeshRenderOnTextureComponent::finalize() {
    destroy();
}

void MeshRenderOnTextureComponent::onEnable(bool value) {
    mMeshRenderOnTexture->getSprite().setActive(value);
}

void MeshRenderOnTextureComponent::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mMeshRenderOnTexture->saveAndLoad(inObj, alloc, mode);
    calcView();
}

void MeshRenderOnTextureComponent::drawMeshOnTexture() const {
    if (!mMeshRenderOnTexture) {
        return;
    }
    mMeshRenderOnTexture->drawMeshOnTexture(mView * mProjection);
}

void MeshRenderOnTextureComponent::draw(const Matrix4& proj) const {
    if (!mMeshRenderOnTexture) {
        return;
    }
    mMeshRenderOnTexture->draw(proj);
}

void MeshRenderOnTextureComponent::changeMeshFromFilePath(const std::string& filePath) {
    mMeshRenderOnTexture->changeMesh(filePath);
    calcView();
}

void MeshRenderOnTextureComponent::changeMesh(const std::string& fileName, const std::string& directoryPath) {
    changeMeshFromFilePath(directoryPath + fileName);
}

void MeshRenderOnTextureComponent::setPositionForTexture(const Vector2& pos) {
    mMeshRenderOnTexture->setPositionForTexture(pos);
}

void MeshRenderOnTextureComponent::setSizeForTexture(const Vector2& size) {
    mMeshRenderOnTexture->setSizeForTexture(size);

}

Sprite& MeshRenderOnTextureComponent::getSprite() const {
    return mMeshRenderOnTexture->getSprite();
}

const IMesh& MeshRenderOnTextureComponent::getMesh() const {
    return mMeshRenderOnTexture->getMesh();
}

const std::string& MeshRenderOnTextureComponent::getFilePath() const {
    return mMeshRenderOnTexture->getFilePath();
}

int MeshRenderOnTextureComponent::getWidth() const {
    return mMeshRenderOnTexture->getWidth();
}

int MeshRenderOnTextureComponent::getHeight() const {
    return mMeshRenderOnTexture->getHeight();
}

void MeshRenderOnTextureComponent::setMeshRenderOnTextureManager(MeshRenderOnTextureManager* manager) {
    mManager = manager;
}

void MeshRenderOnTextureComponent::calcView() {
    SphereHelper::create(mSphereCoverMesh, getMesh());
    mView = CameraHelper::getViewMatrixTakingSphereInCamera(mSphereCoverMesh, getWidth(), getHeight(), 45.f, Vector3::forward, Vector3::up);
}
