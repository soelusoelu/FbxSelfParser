#include "DepthStencilView.h"
#include "DirectXInclude.h"
#include "Texture2D.h"

DepthStencilView::DepthStencilView(const Texture2D& texture2D, const DepthStencilViewDesc* desc)
    : mDepthStencilView(nullptr)
{
    auto dev = MyDirectX::DirectX::instance().device();
    if (desc) {
        const auto& temp = toDSVDesc(desc);
        dev->CreateDepthStencilView(texture2D.texture2D(), &temp, &mDepthStencilView);
    } else {
        dev->CreateDepthStencilView(texture2D.texture2D(), nullptr, &mDepthStencilView);
    }
}

DepthStencilView::~DepthStencilView() = default;

void DepthStencilView::clear(bool depth, bool stencil) const {
    unsigned mask = 0;
    if (depth) {
        mask |= D3D11_CLEAR_DEPTH;
    }
    if (stencil) {
        mask |= D3D11_CLEAR_STENCIL;
    }

    MyDirectX::DirectX::instance().deviceContext()->ClearDepthStencilView(mDepthStencilView.Get(), mask, 1.f, 0);
}

ID3D11DepthStencilView* DepthStencilView::getDepthStencilView() const {
    return mDepthStencilView.Get();
}

D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilView::toDSVDesc(const DepthStencilViewDesc* desc) const {
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
    dsvd.Format = toFormat(desc->format);
    dsvd.ViewDimension = toDimension(desc->viewDimension);
    dsvd.Texture2D.MipSlice = desc->mipSlice;

    return dsvd;
}

D3D11_DSV_DIMENSION DepthStencilView::toDimension(DSVDimension dimension) const {
    static constexpr D3D11_DSV_DIMENSION dimensions[]{
        D3D11_DSV_DIMENSION_TEXTURE2D
    };

    return dimensions[static_cast<unsigned>(dimension)];
}
