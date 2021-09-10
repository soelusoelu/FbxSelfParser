#include "GBuffer.h"
#include "AssetsManager.h"
#include "Window.h"
#include "Shader/ConstantBuffers.h"
#include "Shader/DataTransfer.h"
#include "Shader/ShaderBinder.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../DirectX/DirectXInclude.h"
#include "../Light/LightManager.h"
#include "../Mesh/Vertex.h"
#include "../System/SystemInclude.h"

GBuffer::GBuffer()
    : mSampler(nullptr)
    , mGBufferShaderID(-1)
    , mDefferdShaderID(-1)
    , mVertexBuffer(nullptr)
    , mIndexBuffer(nullptr)
{
}

GBuffer::~GBuffer() = default;

void GBuffer::create() {
    Texture2DDesc desc{};
    ShaderResourceViewDesc srvDesc{};
    RenderTargetViewDesc rtvDesc{};

    //カラー
    desc.width = Window::standardWidth();
    desc.height = Window::standardHeight();
    desc.format = Format::FORMAT_RGBA16_FLOAT;
    desc.usage = Usage::DEFAULT;
    desc.bindFlags =
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET) |
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
    auto texture = std::make_unique<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(*texture, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(*texture, &srvDesc));

    //ノーマル
    desc.format = Format::FORMAT_R10G10B10A2_UNORM;
    auto texture2 = std::make_unique<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(*texture2, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(*texture2, &srvDesc));

    //ポジション
    desc.format = Format::FORMAT_RGBA16_FLOAT;
    auto texture3 = std::make_unique<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(*texture3, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(*texture3, &srvDesc));

    //スペキュラ
    desc.format = Format::FORMAT_RGBA16_FLOAT;
    auto texture4 = std::make_unique<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(*texture4, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(*texture4, &srvDesc));

    //各種生成
    createSampler();
    createShader();
    createVertexBuffer();
    createIndexBuffer();
}

void GBuffer::renderToTexture() {
    auto& dx = MyDirectX::DirectX::instance();

    //シェーダーをセット
    ShaderBinder::bind(mGBufferShaderID);

    //各テクスチャをレンダーターゲットに設定
    RenderTargetView::setRenderTargets(mRenderTargets);

    //クリア
    for (const auto& rt : mRenderTargets) {
        rt->clear();
    }
    dx.clearDepthStencilView();

    //デプステスト有効化
    dx.depthStencilState().depthTest(true);
    //デプスマスク有効化
    dx.depthStencilState().depthMask(true);
    //通常合成
    dx.blendState().normal();
}

void GBuffer::renderFromTexture(const Camera& camera, const LightManager& lightManager) {
    auto& dx = MyDirectX::DirectX::instance();

    //レンダーターゲットを通常に戻す
    dx.setRenderTarget();
    //クリア
    dx.clearRenderTarget();
    dx.clearDepthStencilView();

    //使用するシェーダーは、テクスチャーを参照するシェーダー
    ShaderBinder::bind(mDefferdShaderID);
    //1パス目で作成したテクスチャー3枚をセット
    setShaderResources();
    //サンプラーをセット
    mSampler->setPSSamplers();

    GBufferShaderConstantBuffer cb;
    cb.dirLightDir = lightManager.getDirectionalLight().getDirection();
    cb.dirLightColor = lightManager.getDirectionalLight().getLightColor();
    cb.cameraPos = camera.getPosition();
    cb.ambientLight = lightManager.getAmbientLight();

    //シェーダーにデータ転送
    DataTransfer::transferConstantBuffer(mDefferdShaderID, &cb);

    //スクリーンサイズのポリゴンをレンダー
    dx.setPrimitive(PrimitiveType::TRIANGLE_LIST);
    //バーテックスバッファーをセット
    mVertexBuffer->setVertexBuffer();
    //インデックスバッファをセット
    mIndexBuffer->setIndexBuffer();
    //デプステスト無効化
    dx.depthStencilState().depthTest(false);

    dx.drawIndexed(6);
}

void GBuffer::setShaderResources() const {
    static constexpr unsigned numGBuffer = static_cast<unsigned>(Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        mShaderResourceViews[i]->setVSShaderResources(i);
        mShaderResourceViews[i]->setPSShaderResources(i);
    }
}

void GBuffer::createSampler() {
    //サンプラー作成
    SamplerDesc sd;
    mSampler = std::make_unique<Sampler>(sd);
}

void GBuffer::createShader() {
    //シェーダー生成
    mGBufferShaderID = AssetsManager::instance().createShader("GBuffer.hlsl");
    mDefferdShaderID = AssetsManager::instance().createShader("Deferred.hlsl");
}

void GBuffer::createVertexBuffer() {
    //バーテックスバッファ生成
    static const PosNormUVVertex vertices[] = {
        Vector3(-1.f, -1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(0.f, 1.f),
        Vector3(-1.f, 1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(0.f, 0.f),
        Vector3(1.f, -1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(1.f, 1.f),
        Vector3(1.f, 1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(1.f, 0.f)
    };
    BufferDesc bd;
    bd.oneSize = sizeof(PosNormUVVertex);
    bd.size = bd.oneSize * 4;
    bd.usage = Usage::DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);

    SubResourceDesc sub;
    sub.data = vertices;

    mVertexBuffer = std::make_unique<VertexBuffer>(bd, &sub);
}

void GBuffer::createIndexBuffer() {
    //インデックスバッファ作成
    static constexpr unsigned short indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::INDEX);

    SubResourceDesc sub;
    sub.data = indices;

    mIndexBuffer = std::make_unique<IndexBuffer>(bd, sub);
}
