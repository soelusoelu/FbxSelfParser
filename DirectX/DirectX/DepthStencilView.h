#pragma once

#include "DepthStencilViewDesc.h"
#include "../System/SystemInclude.h"

class Texture2D;

//デプスステンシルテスト中にテクスチャリソースにアクセスするクラス
class DepthStencilView {
public:
    DepthStencilView(const Texture2D& texture2D, const DepthStencilViewDesc* desc = nullptr);
    ~DepthStencilView();

    //深度ステンシルビューをクリアする
    void clear(bool depth = true, bool stencil = false) const;
    //ID3D11DepthStencilViewを取得する
    ID3D11DepthStencilView* getDepthStencilView() const;

private:
    DepthStencilView(const DepthStencilView&) = delete;
    DepthStencilView& operator=(const DepthStencilView&) = delete;

    D3D11_DEPTH_STENCIL_VIEW_DESC toDSVDesc(const DepthStencilViewDesc* desc) const;
    D3D11_DSV_DIMENSION toDimension(DSVDimension dimension) const;

private:
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
};
