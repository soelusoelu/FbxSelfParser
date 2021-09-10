#pragma once

#include "../../DirectX/DirectXInclude.h"

class DataTransfer {
private:
    DataTransfer() = delete;
    ~DataTransfer() = delete;

public:
    //指定のシェーダーの定数バッファにデータを転送する
    static void transferConstantBuffer(
        int shaderID,
        const void* data,
        unsigned constantBufferIndex = 0
    );

    //指定のバッファにデータを転送する
    template<typename T>
    static void transferData(
        const Buffer& inputBuffer,
        const T* data,
        unsigned dataCount = 1
    ) {
        auto buffer = inputBuffer.buffer();

        //開く
        D3D11_MAPPED_SUBRESOURCE mapRes = { 0 };
        map(&mapRes, buffer);

        //データ転送
        auto out = static_cast<T*>(mapRes.pData);
        if (!out) {
            return;
        }
        for (unsigned i = 0; i < dataCount; ++i) {
            auto& dst = out[i];
            const auto& src = data[i];
            memcpy_s(&dst, sizeof(dst), &src, sizeof(src));
        }

        //閉じる
        unmap(buffer);
    }

private:
    //シェーダーに値を渡すための開始・終了処理
    static bool map(
        D3D11_MAPPED_SUBRESOURCE* mapRes,
        ID3D11Buffer* buffer,
        unsigned sub = 0,
        D3D11_MAP type = D3D11_MAP_WRITE_DISCARD,
        unsigned flag = 0
    );
    static void unmap(ID3D11Buffer* buffer, unsigned sub = 0);
};
