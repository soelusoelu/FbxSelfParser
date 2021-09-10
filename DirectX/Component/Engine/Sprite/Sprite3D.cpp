#include "Sprite3D.h"
#include "../../../DirectX/DirectX.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Imgui/imgui.h"
#include "../../../Sprite/SpriteManager.h"
#include "../../../System/AssetsManager.h"
#include "../../../System/Window.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include "../../../System/Shader/DataTransfer.h"
#include "../../../System/Shader/ShaderBinder.h"
#include "../../../System/Texture/Texture.h"
#include "../../../System/Texture/TextureBinder.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"
#include <cassert>

Sprite3D::Sprite3D()
    : Component()
    , mTransform(std::make_unique<Transform3D>())
    , mTextureID(INVALID_ID)
    , mShaderID(-1)
    , mCurrentTextureSize(Vector2::zero)
    , mTextureAspect(Vector2::one)
    , mColor(ColorPalette::white)
    , mAlpha(1.f)
    , mUV(Vector4(0.f, 0.f, 1.f, 1.f))
    , mFileName("")
    , mIsActive(true)
    , mIsBillboard(false)
{
}

Sprite3D::~Sprite3D() = default;

void Sprite3D::awake() {
    //ファイル名を読み込めてたらテクスチャを生成
    if (!mFileName.empty()) {
        mTextureID = AssetsManager::instance().createTexture(mFileName);
        mCurrentTextureSize = texture().getTextureSize();

        //テクスチャのアスペクト比を計算
        calcAspectRatio();
    }

    //シェーダー生成
    mShaderID = AssetsManager::instance().createShader("Texture3D.hlsl");

    //マネージャーに登録する
    addToManager();
}

void Sprite3D::lateUpdate() {
    if (getActive()) {
        const auto& scale = mTransform->getScale();
        mTransform->setScale(scale * Vector3(mTextureAspect, 1.f));
        mTransform->computeMatrix();
        mTransform->setScale(scale);
    }
}

void Sprite3D::finalize() {
    destroy();
}

void Sprite3D::onEnable(bool value) {
    setActive(value);
}

void Sprite3D::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mFileName, "filename", inObj, alloc, mode);
    JsonHelper::getSet(mIsActive, "isActive", inObj, alloc, mode);
    JsonHelper::getSet(mIsBillboard, "isBillboard", inObj, alloc, mode);
    if (mode == FileMode::SAVE) {
        JsonHelper::setVector3(mTransform->getPosition(), "position", inObj, alloc);
        JsonHelper::setVector3(mTransform->getRotation().euler(), "rotation", inObj, alloc);
        JsonHelper::setVector3(mTransform->getScale(), "scale", inObj, alloc);
    } else {
        Vector3 vec3;
        if (JsonHelper::getVector3(vec3, "position", inObj)) {
            mTransform->setPosition(vec3);
        }
        if (JsonHelper::getVector3(vec3, "rotation", inObj)) {
            mTransform->setRotation(vec3);
        }
        if (JsonHelper::getVector3(vec3, "scale", inObj)) {
            mTransform->setScale(vec3);
        }
    }
    JsonHelper::getSet(mColor, "color", inObj, alloc, mode);
    JsonHelper::getSet(mAlpha, "alpha", inObj, alloc, mode);
    JsonHelper::getSet(mUV, "uv", inObj, alloc, mode);
}

void Sprite3D::drawInspector() {
    ImGui::Text("FileName: %s", mFileName.c_str());
    mTransform->drawInspector();
    ImGuiWrapper::colorEdit3("Color", mColor);
    ImGuiWrapper::sliderFloat("Alpha", mAlpha, 0.f, 1.f);
    if (ImGuiWrapper::sliderVector4("UV", mUV, 0.f, 1.f)) {
        setUV(mUV.x, mUV.y, mUV.z, mUV.w);
    }
}

void Sprite3D::draw(const Matrix4& viewProj) const {
    if (!enabledTexture()) {
        return;
    }

    //テクスチャを登録
    TextureBinder::bind(mTextureID);
    //シェーダーを登録
    ShaderBinder::bind(mShaderID);

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb;
    cb.wp = mTransform->getWorldTransform() * viewProj;
    cb.color = Vector4(mColor, mAlpha);
    cb.uv = mUV;

    //シェーダーにデータ転送
    DataTransfer::transferConstantBuffer(mShaderID, &cb);

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(6);
}

void Sprite3D::drawBillboard(const Matrix4& invView, const Matrix4& viewProj) {
    if (!enabledTexture()) {
        return;
    }

    //テクスチャを登録
    TextureBinder::bind(mTextureID);
    //シェーダーを登録
    ShaderBinder::bind(mShaderID);

    //シェーダーのコンスタントバッファーに各種データを渡す
    TextureConstantBuffer cb;

    //ビルボード用ワールド行列計算
    auto world = Matrix4::createTranslation(-mTransform->getPivot());
    world *= Matrix4::createScale(mTransform->getScale() * Vector3(mTextureAspect, 1.f));
    world *= Matrix4::createFromQuaternion(mTransform->getRotation());
    world *= invView;
    world *= Matrix4::createTranslation(mTransform->getPosition());

    //ワールド、射影行列を渡す
    cb.wp = world * viewProj;
    cb.color = Vector4(mColor, mAlpha);
    cb.uv = mUV;

    //シェーダーにデータ転送
    DataTransfer::transferConstantBuffer(mShaderID, &cb);

    //プリミティブをレンダリング
    MyDirectX::DirectX::instance().drawIndexed(6);
}

Transform3D& Sprite3D::transform() const {
    return *mTransform;
}

void Sprite3D::setColor(const Vector3& color) {
    mColor = color;
}

void Sprite3D::setColor(float r, float g, float b) {
    mColor.x = r;
    mColor.y = g;
    mColor.z = b;
}

const Vector3& Sprite3D::getColor() const {
    return mColor;
}

void Sprite3D::setAlpha(float alpha) {
    mAlpha = alpha;
}

float Sprite3D::getAlpha() const {
    return mAlpha;
}

void Sprite3D::setUV(float l, float t, float r, float b) {
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
    mCurrentTextureSize = Vector2(texSize.x * (r - l), texSize.y * (b - t));

    //アスペクト比再計算
    calcAspectRatio();
}

const Vector4& Sprite3D::getUV() const {
    return mUV;
}

void Sprite3D::setActive(bool value) {
    mIsActive = value;
}

bool Sprite3D::getActive() const {
    return mIsActive;
}

void Sprite3D::setTextureFromFileName(const std::string& fileName) {
    mTextureID = AssetsManager::instance().createTexture(fileName);

    //各種初期化
    mCurrentTextureSize = texture().getTextureSize();
    mColor = ColorPalette::white;
    mAlpha = 1.f;
    mUV = Vector4(0.f, 0.f, 1.f, 1.f);

    //テクスチャのアスペクト比を計算
    calcAspectRatio();

    //ファイル名変更
    mFileName = fileName;
}

const Texture& Sprite3D::texture() const {
    return AssetsManager::instance().getTextureFromID(mTextureID);
}

int Sprite3D::getTextureID() const {
    return mTextureID;
}

const Vector2& Sprite3D::getTextureAspect() const {
    return mTextureAspect;
}

const Shader& Sprite3D::shader() const {
    return AssetsManager::instance().getShaderFormID(mShaderID);
}

int Sprite3D::getShaderID() const {
    return mShaderID;
}

const std::string& Sprite3D::fileName() const {
    return mFileName;
}

void Sprite3D::setBillboard(bool value) {
    mIsBillboard = value;
}

bool Sprite3D::isBillboard() const {
    return mIsBillboard;
}

void Sprite3D::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void Sprite3D::addToManager() {
    if (mSpriteManager) {
        mSpriteManager->add3D(shared_from_this());
    }
}

void Sprite3D::calcAspectRatio() {
    if (!enabledTexture()) {
        return;
    }

    //x軸を基準にアスペクト比を求める
    mTextureAspect.x = mCurrentTextureSize.x / mCurrentTextureSize.y;
}

bool Sprite3D::enabledTexture() const {
    return (mTextureID != INVALID_ID);
}
