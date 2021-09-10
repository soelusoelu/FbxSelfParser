#include "SpriteInstancingDrawer.h"
#include "Sprite.h"
#include "../System/Shader/DataTransfer.h"
#include "../System/Shader/ShaderBinder.h"
#include "../System/Texture/Texture.h"
#include "../System/Texture/TextureBinder.h"
#include "../Transform/Transform2D.h"

SpriteInstancingDrawer::SpriteInstancingDrawer()
    : mInputBuffer(nullptr)
{
}

SpriteInstancingDrawer::~SpriteInstancingDrawer() = default;

void SpriteInstancingDrawer::initialize() {
    BufferDesc bd{};
    bd.size = sizeof(TextureConstantBuffer) * MAX_INSTANCE;
    bd.usage = Usage::DYNAMIC;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);
    bd.cpuAccessFlags = static_cast<unsigned>(BufferCPUAccessFlag::WRITE);

    mInputBuffer = std::make_unique<VertexBuffer>(bd);
}

void SpriteInstancingDrawer::add(const Sprite& sprite, const Matrix4& proj) {
    //スプライトから描画に必要なデータを抜き出す
    TextureConstantBuffer tcb{};
    tcb.wp = sprite.transform().getWorldTransform() * proj;
    tcb.color = Vector4(sprite.getColor(), sprite.getAlpha());
    tcb.uv = sprite.getUV();

    //新しいインスタンスデータを登録
    mInstancingData.emplace_back(tcb);
}

void SpriteInstancingDrawer::instancingDraw(const Sprite& sprite) const {
    if (mInstancingData.empty()) {
        return;
    }
    if (!sprite.getActive() || sprite.isDead()) {
        return;
    }

    //インスタンシング用頂点バッファを登録する
    auto texBuf = Texture::vertexBuffer;
    ID3D11Buffer* buffers[NUM_VERTEX_BUFFER] = { texBuf->buffer(), mInputBuffer->buffer() };
    unsigned strides[NUM_VERTEX_BUFFER] = { texBuf->desc().oneSize, sizeof(TextureConstantBuffer) };
    texBuf->setVertexBuffer(NUM_VERTEX_BUFFER, buffers, strides);

    //テクスチャを登録する
    TextureBinder::bind(sprite.getTextureID());
    //シェーダーを登録する
    ShaderBinder::bind(sprite.getShaderID());

    //シェーダーにデータ転送
    DataTransfer::transferData(
        *mInputBuffer,
        mInstancingData.data(),
        mInstancingData.size()
    );

    //描画
    MyDirectX::DirectX::instance().drawIndexedInstanced(6, mInstancingData.size());
}

void SpriteInstancingDrawer::clear() {
    mInstancingData.clear();
}
