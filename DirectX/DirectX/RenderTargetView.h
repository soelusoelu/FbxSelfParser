#pragma once

#include "RenderTargetViewDesc.h"
#include "../System/SystemInclude.h"
#include <memory>
#include <vector>

class Texture2D;
class DepthStencilView;

class RenderTargetView {
public:
    RenderTargetView(const Texture2D& texture2D, const RenderTargetViewDesc* desc = nullptr);
    ~RenderTargetView();
    //デフォルトの深度ステンシルビューを使用して、レンダーターゲットを設定する
    void setRenderTarget() const;
    //深度ステンシルビューを使用して、レンダーターゲットを設定する
    void setRenderTarget(const DepthStencilView& depthStencilView) const;
    //レンダーターゲットをクリアする
    void clear(float r = 0.f, float g = 0.f, float b = 1.f, float a = 1.f) const;

    //デフォルトの深度ステンシルビューを使用して、複数のレンダーターゲットを設定する
    static void setRenderTargets(
        const std::vector<std::unique_ptr<RenderTargetView>>& targets
    );
    //深度ステンシルビューを使用して、レンダーターゲットを設定する
    static void setRenderTargets(
        const std::vector<std::unique_ptr<RenderTargetView>>& targets,
        const DepthStencilView& depthStencilView
    );

private:
    D3D11_RENDER_TARGET_VIEW_DESC toRTVDesc(const RenderTargetViewDesc* desc) const;
    D3D11_RTV_DIMENSION toDimension(RTVDimension dimension) const;

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
};
