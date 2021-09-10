#pragma once

#include "BufferDesc.h"
#include "SamplerDesc.h"
#include "SubResourceDesc.h"
#include "Texture2DDesc.h"
#include "../System/SystemInclude.h"
#include <dxgi1_6.h>
#include <memory>
#include <vector>

#pragma comment(lib, "dxgi.lib")

enum class PrimitiveType {
    POINT_LIST,
    LINE_LIST,
    LINE_STRIP,
    TRIANGLE_LIST,
    TRIANGLE_STRIP
};

class BlendState;
class DepthStencilState;
class DepthStencilView;
class RasterizerState;
class RenderTargetView;

namespace MyDirectX {

class DirectX {
private:
    DirectX();
public:
    ~DirectX();
    static DirectX& instance();
    void initialize(const HWND& hWnd);
    void finalize();

    //DirectXデバイスを取得する
    ID3D11Device* device() const;
    //DirectXデバイスコンテキストを取得する
    ID3D11DeviceContext* deviceContext() const;
    //ブレンドステートを取得する
    BlendState& blendState() const;
    //深度ステンシルステートを取得する
    DepthStencilState& depthStencilState() const;
    //深度ステンシルビューを取得する
    DepthStencilView& depthStencilView() const;
    //ラスタライザステートを取得する
    RasterizerState& rasterizerState() const;

    //ビューポートを設定する
    void setViewport(
        float width,
        float height,
        float x = 0.f,
        float y = 0.f
    ) const;
    //レンダーターゲットをバックバッファに設定する
    void setRenderTarget() const;
    //レンダーターゲットをデバッグ画面に設定する
    void setDebugRenderTarget() const;
    //描画形状を設定する
    void setPrimitive(PrimitiveType primitive) const;
    //リソースを更新する
    void updateSubresource(
        ID3D11Resource* outResource,
        const void* inData,
        unsigned outSubresource = 0,
        const D3D11_BOX* outBox = nullptr,
        unsigned inRowPitch = 0,
        unsigned inDepthPitch = 0
    );

    //頂点数を指定して描画する
    void draw(unsigned numVertex, unsigned start = 0);
    //インデックスバッファを使用して描画する
    void drawIndexed(
        unsigned numIndices,
        unsigned startIndex = 0,
        int startVertex = 0
    );
    //インデックスバッファを使用したインスタンシング描画
    void drawIndexedInstanced(
        unsigned numIndices,
        unsigned numInstance,
        unsigned startIndex = 0,
        int startVertex = 0,
        unsigned startInstance = 0
    );

    //バックバッファとデバッグ画面をクリアする
    void clearRenderTarget(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f) const;
    //深度ステンシルビューをクリアする
    void clearDepthStencilView(bool depth = true, bool stencil = false);
    //バックバッファを切り替える
    void present();

private:
    DirectX(const DirectX&) = delete;
    DirectX& operator=(const DirectX&) = delete;

    void createDevice();
    void createSwapChain(const HWND& hWnd);
    void createRenderTargetView();
    void createDepthStencilView();
    D3D11_PRIMITIVE_TOPOLOGY toPrimitiveMode(PrimitiveType primitive) const;

private:
    static inline DirectX* mInstance = nullptr;

    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
    Microsoft::WRL::ComPtr<IDXGIFactory6> mDXGIFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;

    std::unique_ptr<RenderTargetView> mRenderTargetView;
    std::unique_ptr<RenderTargetView> mDebugRenderTargetView;
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthStencilState;
    std::shared_ptr<DepthStencilView> mDepthStencilView;
    std::shared_ptr<RasterizerState> mRasterizerState;
};

}
