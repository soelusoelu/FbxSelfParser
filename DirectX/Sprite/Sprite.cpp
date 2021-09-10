#include "Sprite.h"
#include "SpriteManager.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/AssetsManager.h"
#include "../System/shader/ConstantBuffers.h"
#include "../System/shader/DataTransfer.h"
#include "../System/shader/ShaderBinder.h"
#include "../System/Texture/Texture.h"
#include "../System/Texture/TextureBinder.h"
#include "../Transform/Transform2D.h"
#include <cassert>
#include <vector>

Sprite::Sprite()
    : mTransform(std::make_unique<Transform2D>())
    , mTextureID(INVALID_ID)
    , mShaderID(AssetsManager::instance().createShader("Texture.hlsl"))
    , mColor(ColorPalette::white)
    , mAlpha(1.f)
    , mUV(0.f, 0.f, 1.f, 1.f)
    , mFileName()
    , mIsActive(true)
{
}

Sprite::Sprite(const std::string& fileName)
    : Sprite()
{
    //テクスチャ生成
    setTextureFromFileName(fileName);
}

Sprite::Sprite(const std::string& fileName, const std::string& shaderName)
    : Sprite(fileName)
{
    mShaderID = AssetsManager::instance().createShader(shaderName);
}

Sprite::~Sprite() = default;

void Sprite::computeWorldTransform() {
    if (getActive()) {
        mTransform->computeWorldTransform();
    }
}

void Sprite::draw(const Matrix4& proj) const {
    if (!enabledTexture()) {
        return;
    }

    //テクスチャを登録
    TextureBinder::bind(mTextureID);
    //シェーダーを登録
    ShaderBinder::bind(mShaderID);

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb{};
    cb.wp = mTransform->getWorldTransform() * proj;
    cb.color = Vector4(mColor, mAlpha);
    cb.uv = mUV;

    //シェーダーにデータ転送
    DataTransfer::transferConstantBuffer(mShaderID, &cb);

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(6);
}

Transform2D& Sprite::transform() const {
    return *mTransform;
}

void Sprite::setColor(const Vector3& color) {
    mColor = color;
}

void Sprite::setColor(float r, float g, float b) {
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}

const Vector3& Sprite::getColor() const {
    return mColor;
}

void Sprite::setAlpha(float alpha) {
    mAlpha = alpha;
}

float Sprite::getAlpha() const {
    return mAlpha;
}

void Sprite::setUV(float l, float t, float r, float b) {
    assert(0.f <= l || l <= 1.f);
    assert(0.f <= t || t <= 1.f);
    assert(l <= r || r <= 1.f);
    assert(t <= b || b <= 1.f);

    mUV.x = l;
    mUV.y = t;
    mUV.z = r;
    mUV.w = b;

    //サイズ修正
    const auto& texSize = texture().getTextureSize();
    Vector2 size = Vector2(texSize.x * (r - l), texSize.y * (b - t));

    //テクスチャサイズを変更したことを通知
    mTransform->setSize(size);
}

const Vector4& Sprite::getUV() const {
    return mUV;
}

const Vector2& Sprite::getTextureSize() const {
    return texture().getTextureSize();
}

void Sprite::setActive(bool value) {
    mIsActive = value;
}

bool Sprite::getActive() const {
    return mIsActive;
}

void Sprite::setTextureFromFileName(const std::string& fileName) {
    mTextureID = AssetsManager::instance().createTexture(fileName);

    //Transformに通知
    mTransform->setSize(texture().getTextureSize());

    //ファイル名変更
    mFileName = fileName;
}

void Sprite::setTexture(const std::shared_ptr<Texture>& texture) {
    mTextureID = AssetsManager::instance().addTexture(texture);

    //Transformに通知
    mTransform->setSize(texture->getTextureSize());

    //ファイル名変更
    mFileName.clear();
}

const Texture& Sprite::texture() const {
    return AssetsManager::instance().getTextureFromID(mTextureID);
}

int Sprite::getTextureID() const {
    return mTextureID;
}

const Shader& Sprite::shader() const {
    return AssetsManager::instance().getShaderFormID(mShaderID);
}

int Sprite::getShaderID() const {
    return mShaderID;
}

const std::string& Sprite::fileName() const {
    return mFileName;
}

void Sprite::setSpriteManager(SpriteManager* manager) {
    mManager = manager;
}

SpriteManager& Sprite::getSpriteManager() const {
    return *mManager;
}

bool Sprite::enabledTexture() const {
    return (mTextureID != INVALID_ID);
}
