#include "AssetsRenderTexture.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../System/Texture/MeshRenderOnTexture.h"

AssetsRenderTexture::AssetsRenderTexture(const std::string& filePath, int size)
    : mTexture(std::make_unique<MeshRenderOnTexture>(filePath, size, size))
{
}

AssetsRenderTexture::~AssetsRenderTexture() = default;

void AssetsRenderTexture::drawMesh() const {
    Sphere sphere;
    SphereHelper::create(sphere, mTexture->getMesh());
    const auto& view = CameraHelper::getViewMatrixTakingSphereInCamera(sphere, 1, 1, FOV, Vector3::normalize(DIRECTION), Vector3::up);
    const auto& proj = Matrix4::createPerspectiveFOV(1.f, FOV, NEAR_CLIP, FAR_CLIP);
    mTexture->drawMeshOnTexture(view * proj);
}

MeshRenderOnTexture& AssetsRenderTexture::getTexture() const {
    return *mTexture;
}
