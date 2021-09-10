#include "VertexBuffer.h"
#include "DirectX.h"

VertexBuffer::VertexBuffer(const BufferDesc& desc, const SubResourceDesc* data)
    : Buffer(desc, data)
{
}

VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::setVertexBuffer() {
    setVertexBuffer(1, mBuffer.GetAddressOf(), &mDesc.oneSize);
}

void VertexBuffer::setVertexBuffer(
    unsigned numStream,
    ID3D11Buffer* const* buffers,
    const unsigned* strides
) {
    /* IASetVertexBuffers
        使い始めのスロット番号
        頂点バッファ配列の要素数
        頂点バッファ配列の先頭ポインタ
        INPUT_ELEMENT_DESC構造体のサイズが入った配列への先頭ポインタ(stride(読み込み単位)として扱うため)
        頂点バッファ配列の各頂点バッファの頭出しをするオフセット値の配列
    */
    static constexpr unsigned offset[2] = { 0, 0 };
    MyDirectX::DirectX::instance().deviceContext()->IASetVertexBuffers(
        0,
        numStream,
        buffers,
        strides,
        offset
    );
}

void VertexBuffer::updateVertexBuffer(const void* newVertices) {
    MyDirectX::DirectX::instance().updateSubresource(mBuffer.Get(), newVertices);
}
