#include "TextureBinder.h"
#include "Texture.h"
#include "../AssetsManager.h"
#include "../../DirectX/DirectXInclude.h"

void TextureBinder::bind(int id) {
    //前回描画時のテクスチャと違うならバインドする
    if (id != mPreviousTextureID) {
        mPreviousTextureID = id;

        //IDからテクスチャを取得
        const auto& tex = AssetsManager::instance().getTextureFromID(id);

        //テクスチャバインド
        const auto& srv = tex.getShaderResourceView();
        srv.setVSShaderResources();
        srv.setPSShaderResources();

        //サンプラーバインド
        const auto& sampler = tex.getSampler();
        sampler.setVSSamplers();
        sampler.setPSSamplers();
    }
}
