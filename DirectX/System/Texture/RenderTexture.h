#pragma once

#include "../../DirectX/Format.h"
#include "../../DirectX/Texture2DDesc.h"
#include <memory>

class Texture2D;
class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;

//描画可能テクスチャ
class RenderTexture {
public:
    RenderTexture(int width, int height, Format depthFormat = Format::FORMAT_D24_UNORM_S8_UINT, Format textureFormat = Format::FORMAT_RGBA8_UNORM);
    ~RenderTexture();

    //描画前処理
    void drawBegin(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f, bool depth = true, bool stencil = false);
    //描画終了処理
    void drawEnd() const;
    //シェーダーリソースビューをGPUに送る
    void transferShaderResourceView(unsigned constantBufferIndex);
    //レンダーターゲットテクスチャの使用後処理
    void drawEndTexture();
    //シェーダーリソースビューを取得する
    const std::shared_ptr<ShaderResourceView>& getShaderResourceView() const;

private:
    RenderTexture(const RenderTexture&) = delete;
    RenderTexture& operator=(const RenderTexture&) = delete;

    //テクスチャディスクリプタを作成する
    void createDepthDesc(Texture2DDesc& desc, int width, int height, Format textureFormat) const;
    //テクスチャを作成する
    void createDepthTexture(const Texture2DDesc& desc);
    //レンダーターゲットビューを作成する
    void createDepthRenderTargetView(Format textureFormat);
    //深度ステンシルビューを作成する
    void createDepthStencilView(const Texture2DDesc& desc, Format depthFormat);
    //シェーダーリソースビューを作成する
    void createDepthShaderResourceView(Format textureFormat);

private:
    std::unique_ptr<Texture2D> mTexture;
    std::unique_ptr<RenderTargetView> mRenderTargetView;
    std::unique_ptr<DepthStencilView> mDepthStencilView;
    std::shared_ptr<ShaderResourceView> mShaderResourceView;
    unsigned mTextureConstBufferIndex;
};
