#pragma once

#include "Buffer.h"
#include "BufferDesc.h"
#include "SubResourceDesc.h"

class VertexBuffer
    : public Buffer
{
public:
    VertexBuffer(const BufferDesc& desc, const SubResourceDesc* data = nullptr);
    ~VertexBuffer();
    //頂点バッファをGPUに設定する(基本版)
    void setVertexBuffer();
    //頂点バッファをGPUに設定する(拡張版)
    void setVertexBuffer(
        unsigned numStream,
        ID3D11Buffer* const* buffers,
        const unsigned* strides
    );
    //頂点バッファを更新する
    void updateVertexBuffer(const void* newVertices);

private:
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
};
