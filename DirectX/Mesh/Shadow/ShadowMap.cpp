#include "ShadowMap.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../Component/Engine/Mesh/MeshShader.h"
#include "../../Component/Engine/Sprite/SpriteComponent.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Imgui/imgui.h"
#include "../../Math/Math.h"
#include "../../System/AssetsManager.h"
#include "../../System/Window.h"
#include "../../System/Shader/DataTransfer.h"
#include "../../System/Shader/ShaderBinder.h"
#include "../../System/Texture/Texture.h"
#include "../../System/Texture/RenderTexture.h"
#include "../../Transform/Transform2D.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/JsonHelper.h"

ShadowMap::ShadowMap()
    : FileOperator("ShadowMap")
    , mDepthTextureCreateShaderID(-1)
    , mRenderTexture(nullptr)
    , mShadowTextureSize(0)
    , mLightDistance(0.f)
    , mNearClip(0.f)
    , mFarClip(0.f)
{
}

ShadowMap::~ShadowMap() = default;

void ShadowMap::initialize() {
    mDepthTextureCreateShaderID = AssetsManager::instance().createShader("ShadowDepthTextureCreater.hlsl");
    mRenderTexture = std::make_unique<RenderTexture>(mShadowTextureSize, mShadowTextureSize, Format::FORMAT_D24_UNORM_S8_UINT, Format::FORMAT_R16_UNORM);

    //const auto& s = getComponent<SpriteComponent>();
    //const auto& tex = std::make_shared<Texture>(mRenderTexture->getShaderResourceView(), Vector2(Window::width(), Window::height()));
    //s->setTexture(tex);
    //auto& t = s->transform();
    //t.setScale(0.5f);
}

void ShadowMap::drawBegin(const Vector3& dirLightDirection) {
    mRenderTexture->drawBegin(1.f, 1.f, 1.f, 1.f, true, false);

    //シェーダー登録
    ShaderBinder::bind(mDepthTextureCreateShaderID);

    //ライトビュー計算
    mShadowConstBuffer.lightView = Matrix4::createLookAt(-dirLightDirection * mLightDistance, dirLightDirection, Vector3::up);
    mShadowConstBuffer.lightProj = Matrix4::createOrtho(Window::width(), Window::height(), mNearClip, mFarClip);
}

void ShadowMap::draw(const MeshRenderer& renderer) const {
    SimpleMeshConstantBuffer smcb{};
    smcb.wvp = renderer.transform().getWorldTransform() * mShadowConstBuffer.lightView * mShadowConstBuffer.lightProj;
    DataTransfer::transferConstantBuffer(mDepthTextureCreateShaderID, &smcb);

    const auto& meshComp = renderer.getMeshComponent();
    const auto& drawer = meshComp.getDrawer();
    const auto loopCount = meshComp.getMesh()->getMeshCount();
    for (unsigned i = 0; i < loopCount; ++i) {
        drawer->draw(i);
    }
}

void ShadowMap::setShadowConstantBuffer(MeshRenderer& renderer) {
    renderer.getMeshShader().setTransferData(&mShadowConstBuffer, 2);
}

void ShadowMap::drawEnd() const {
    mRenderTexture->drawEnd();
}

void ShadowMap::transferShadowTexture(unsigned constantBufferIndex) {
    mRenderTexture->transferShaderResourceView(constantBufferIndex);
}

void ShadowMap::drawEndShadowTexture() {
    mRenderTexture->drawEndTexture();
}

void ShadowMap::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mShadowTextureSize, "shadowTextureSize", inObj, alloc, mode);
    JsonHelper::getSet(mLightDistance, "lightDistance", inObj, alloc, mode);
    JsonHelper::getSet(mNearClip, "nearClip", inObj, alloc, mode);
    JsonHelper::getSet(mFarClip, "farClip", inObj, alloc, mode);
}
