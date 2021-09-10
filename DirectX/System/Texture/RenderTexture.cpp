#include "RenderTexture.h"
#include "../Window.h"
#include "../Shader/Shader.h"
#include "../../DirectX/DirectXInclude.h"

RenderTexture::RenderTexture(int width, int height, Format depthFormat, Format textureFormat)
    : mTexture(nullptr)
    , mRenderTargetView(nullptr)
    , mDepthStencilView(nullptr)
    , mShaderResourceView(nullptr)
    , mTextureConstBufferIndex(0)
{
    Texture2DDesc desc{};
    createDepthDesc(desc, width, height, textureFormat);
    createDepthTexture(desc);
    createDepthRenderTargetView(desc.format);
    createDepthStencilView(desc, depthFormat);
    createDepthShaderResourceView(desc.format);
}

RenderTexture::~RenderTexture() = default;

void RenderTexture::drawBegin(float r, float g, float b, float a, bool depth, bool stencil) {
    auto& dx = MyDirectX::DirectX::instance();

    //レンダーターゲットを設定する
    mRenderTargetView->setRenderTarget(*mDepthStencilView);
    //レンダーターゲットをクリアする
    mRenderTargetView->clear(r, g, b, a);
    //深度バッファクリア
    mDepthStencilView->clear(depth, stencil);
    //ビューポート設定
    const auto& texDesc = mTexture->desc();
    dx.setViewport(texDesc.width, texDesc.height);
}

void RenderTexture::drawEnd() const {
    auto& dx = MyDirectX::DirectX::instance();

    //レンダーターゲットをバックバッファに戻す
    dx.setRenderTarget();
    //ビューポート設定
    dx.setViewport(Window::width(), Window::height());
}

void RenderTexture::transferShaderResourceView(unsigned constantBufferIndex) {
    mShaderResourceView->setPSShaderResources(constantBufferIndex);
    mTextureConstBufferIndex = constantBufferIndex;
}

void RenderTexture::drawEndTexture() {
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nullView = nullptr;
    MyDirectX::DirectX::instance().deviceContext()->PSSetShaderResources(mTextureConstBufferIndex, 1, nullView.GetAddressOf());
}

const std::shared_ptr<ShaderResourceView>& RenderTexture::getShaderResourceView() const {
    return mShaderResourceView;
}

void RenderTexture::createDepthDesc(Texture2DDesc& desc, int width, int height, Format textureFormat) const {
    //テクスチャディスクリプタ
    desc.width = width;
    desc.height = height;
    desc.format = textureFormat;
    desc.usage = Usage::DEFAULT;
    desc.bindFlags =
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET)
        | static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
}

void RenderTexture::createDepthTexture(const Texture2DDesc& desc) {
    //テクスチャ作成
    mTexture = std::make_unique<Texture2D>(desc);
}

void RenderTexture::createDepthRenderTargetView(Format textureFormat) {
    //レンダーターゲットビュー
    RenderTargetViewDesc rtvDesc{};
    rtvDesc.format = textureFormat;

    //レンダーターゲットビュー作成
    mRenderTargetView = std::make_unique<RenderTargetView>(*mTexture, &rtvDesc);
}

void RenderTexture::createDepthStencilView(const Texture2DDesc& desc, Format depthFormat) {
    //深度ステンシルビュー
    Texture2DDesc dsDesc{};
    memcpy_s(&dsDesc, sizeof(dsDesc), &desc, sizeof(desc));
    dsDesc.format = depthFormat;
    dsDesc.bindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_DEPTH_STENCIL);
    const auto& depthStencilTexture = std::make_unique<Texture2D>(dsDesc);

    //深度ステンシルビュー作成
    mDepthStencilView = std::make_unique<DepthStencilView>(*depthStencilTexture);
}

void RenderTexture::createDepthShaderResourceView(Format textureFormat) {
    //シェーダーリソースビュー
    ShaderResourceViewDesc srvDesc{};
    srvDesc.format = textureFormat;

    //シェーダーリソースビュー作成
    mShaderResourceView = std::make_shared<ShaderResourceView>(*mTexture, &srvDesc);
}
