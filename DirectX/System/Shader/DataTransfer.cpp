#include "DataTransfer.h"
#include "Shader.h"
#include "../AssetsManager.h"

void DataTransfer::transferConstantBuffer(
    int shaderID,
    const void* data,
    unsigned constantBufferIndex
) {
    const auto& shader = AssetsManager::instance().getShaderFormID(shaderID);
    auto dc = MyDirectX::DirectX::instance().deviceContext();

    //定数バッファを設定する
    const auto& constantBuffer = shader.getConstantBuffer(constantBufferIndex);
    auto buffer = constantBuffer.buffer();
    auto bufferAddres = constantBuffer.bufferAddres();
    dc->VSSetConstantBuffers(constantBufferIndex, 1, bufferAddres);
    dc->PSSetConstantBuffers(constantBufferIndex, 1, bufferAddres);

    D3D11_MAPPED_SUBRESOURCE mapRes = { 0 };
    //開く
    map(&mapRes, buffer);
    //データ転送
    memcpy(mapRes.pData, data, mapRes.RowPitch);
    //閉じる
    unmap(buffer);
}

bool DataTransfer::map(
    D3D11_MAPPED_SUBRESOURCE* mapRes,
    ID3D11Buffer* buffer,
    unsigned sub,
    D3D11_MAP type,
    unsigned flag
) {
    return SUCCEEDED(MyDirectX::DirectX::instance().deviceContext()->Map(buffer, sub, type, flag, mapRes));
}

void DataTransfer::unmap(ID3D11Buffer* buffer, unsigned sub) {
    MyDirectX::DirectX::instance().deviceContext()->Unmap(buffer, sub);
}
