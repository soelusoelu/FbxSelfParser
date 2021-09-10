#pragma once

#include "../../Math/Math.h"
#include <memory>

//頂点の構造体
struct TextureVertex {
    Vector3 pos; //位置
    Vector2 uv; //テクスチャ座標
};

class VertexBuffer;
class IndexBuffer;
class ShaderResourceView;
class Sampler;

class Texture {
public:
    Texture();
    Texture(const std::shared_ptr<ShaderResourceView>& view, const Vector2& textureSize);
    virtual ~Texture();
    //テクスチャサイズを返す
    const Vector2& getTextureSize() const;
    //シェーダーリソースビューを取得する
    const ShaderResourceView& getShaderResourceView() const;
    //サンプラーを取得する
    const Sampler& getSampler() const;

    //終了処理
    static void finalize();

private:
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void createVertexBuffer();
    void createIndexBuffer();
    void createSampler();

public:
    static inline VertexBuffer* vertexBuffer = nullptr;
    static inline VertexBuffer* vertexBuffer3D = nullptr;
    static inline IndexBuffer* indexBuffer = nullptr;

protected:
    std::shared_ptr<ShaderResourceView> mShaderResourceView;
    std::unique_ptr<Sampler> mSampler;
    Vector2 mTextureSize;
};
