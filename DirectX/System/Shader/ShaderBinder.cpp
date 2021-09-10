#include "ShaderBinder.h"
#include "Shader.h"
#include "../AssetsManager.h"
#include "../../DirectX/DirectXInclude.h"

void ShaderBinder::bind(int id) {
    //前回描画時のテクスチャと違うならバインドする
    if (id != mPreviousShaderID) {
        mPreviousShaderID = id;

        //IDからシェーダーを取得
        const auto& s = AssetsManager::instance().getShaderFormID(id);

        //シェーダーバインド
        auto dc = MyDirectX::DirectX::instance().deviceContext();
        dc->VSSetShader(s.getVertexShader(), nullptr, 0);
        dc->PSSetShader(s.getPixelShader(), nullptr, 0);

        //頂点レイアウトバインド
        dc->IASetInputLayout(s.getVertexLayout().layout());
    }
}
