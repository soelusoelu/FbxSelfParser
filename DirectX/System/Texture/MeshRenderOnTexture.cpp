#include "MeshRenderOnTexture.h"
#include "RenderTexture.h"
#include "Texture.h"
#include "../AssetsManager.h"
#include "../Shader/ConstantBuffers.h"
#include "../Shader/DataTransfer.h"
#include "../Shader/ShaderBinder.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Mesh/Mesh.h"
#include "../../Mesh/MeshCommonShaderSetter.h"
#include "../../Sprite/Sprite.h"
#include "../../Transform/Transform2D.h"
#include "../../Utility/JsonHelper.h"

MeshRenderOnTexture::MeshRenderOnTexture()
    : mRenderTexture(nullptr)
    , mSprite(std::make_unique<Sprite>())
    , mMesh(nullptr)
    , mMeshShaderID(AssetsManager::instance().createShader("SimpleMeshTexture.hlsl"))
    , mFilePath()
    , mWidth(DEFAULT_SPRITE_WIDTH)
    , mHeight(DEFAULT_SPRITE_HEIGHT)
{
}

MeshRenderOnTexture::MeshRenderOnTexture(const std::string& filePath, int width, int height)
    : mRenderTexture(std::make_unique<RenderTexture>(width, height, Format::FORMAT_D16_UNORM, Format::FORMAT_RGBA8_UNORM))
    , mSprite(std::make_unique<Sprite>())
    , mMesh(AssetsManager::instance().createMeshFromFilePath(filePath))
    , mMeshShaderID(AssetsManager::instance().createShader("SimpleMeshTexture.hlsl"))
    , mFilePath(filePath)
    , mWidth(width)
    , mHeight(height)
{
    const auto& tex = std::make_shared<Texture>(mRenderTexture->getShaderResourceView(), Vector2(width, height));
    mSprite->setTexture(tex);
}

//MeshRenderOnTexture::MeshRenderOnTexture(const std::string& fileName, const std::string& directoryPath, int width, int height)
//    : MeshRenderOnTexture(directoryPath + fileName, width, height)
//{
//}

void MeshRenderOnTexture::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mFilePath, "filePath", inObj, alloc, mode);
    JsonHelper::getSet(mWidth, "width", inObj, alloc, mode);
    JsonHelper::getSet(mHeight, "height", inObj, alloc, mode);

    mRenderTexture = std::make_unique<RenderTexture>(mWidth, mHeight, Format::FORMAT_D16_UNORM, Format::FORMAT_RGBA8_UNORM);
    mMesh = AssetsManager::instance().createMeshFromFilePath(mFilePath);
    const auto& tex = std::make_shared<Texture>(mRenderTexture->getShaderResourceView(), Vector2(mWidth, mHeight));
    mSprite->setTexture(tex);
}

MeshRenderOnTexture::~MeshRenderOnTexture() = default;

void MeshRenderOnTexture::drawMeshOnTexture(const Matrix4& viewProj) const {
    //レンダーテクスチャへの書き込み前処理
    mRenderTexture->drawBegin();

    ShaderBinder::bind(mMeshShaderID);

    SimpleMeshConstantBuffer meshcb = { Matrix4::identity * viewProj };
    DataTransfer::transferConstantBuffer(mMeshShaderID, &meshcb, 0);

    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        MaterialConstantBuffer matcb{};
        MeshCommonShaderSetter::setMaterial(matcb, mMesh->getMaterial(i));
        DataTransfer::transferConstantBuffer(mMeshShaderID, &matcb, 1);

        mMesh->draw(i);
    }

    //レンダーテクスチャへの書き込み後処理
    mRenderTexture->drawEnd();
}

void MeshRenderOnTexture::draw(const Matrix4& proj) const {
    mRenderTexture->transferShaderResourceView(1);
    mSprite->draw(proj);
    mRenderTexture->drawEndTexture();
}

void MeshRenderOnTexture::changeMeshFromFilePath(const std::string& filePath) {
    mFilePath = filePath;
    mMesh = AssetsManager::instance().createMeshFromFilePath(filePath);
}

void MeshRenderOnTexture::changeMesh(const std::string& fileName, const std::string& directoryPath) {
    changeMeshFromFilePath(directoryPath + fileName);
}

void MeshRenderOnTexture::setPositionForTexture(const Vector2& pos) {
    mSprite->transform().setPosition(pos);
    mSprite->computeWorldTransform();
}

void MeshRenderOnTexture::setSizeForTexture(const Vector2& size) {
    mWidth = static_cast<int>(size.x + 0.5f);
    mHeight = static_cast<int>(size.y + 0.5f);
    mSprite->transform().setSize(size);
    mSprite->computeWorldTransform();
}

Sprite& MeshRenderOnTexture::getSprite() const {
    return *mSprite;
}

const IMesh& MeshRenderOnTexture::getMesh() const {
    return *mMesh;
}

const std::string& MeshRenderOnTexture::getFilePath() const {
    return mFilePath;
}

int MeshRenderOnTexture::getWidth() const {
    return mWidth;
}

int MeshRenderOnTexture::getHeight() const {
    return mHeight;
}
