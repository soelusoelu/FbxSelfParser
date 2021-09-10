#pragma once

#include "../DirectX/DirectXInclude.h"
#include "../Math/Math.h"
#include "../System/Shader/ConstantBuffers.h"
#include <memory>
#include <vector>

class Sprite;

class SpriteInstancingDrawer {
public:
    SpriteInstancingDrawer();
    ~SpriteInstancingDrawer();
    //初期化
    void initialize();
    //インスタンスデータを登録する
    void add(const Sprite& sprite, const Matrix4& proj);
    //インスタンスを描画する
    void instancingDraw(const Sprite& sprite) const;
    //全バッファ削除
    void clear();

private:
    SpriteInstancingDrawer(const SpriteInstancingDrawer&) = delete;
    SpriteInstancingDrawer& operator=(const SpriteInstancingDrawer&) = delete;

private:
    //インスタンシングデータ
    std::vector<TextureConstantBuffer> mInstancingData;
    //インスタンスごとの固有データ用バッファ
    std::unique_ptr<VertexBuffer> mInputBuffer;

    //一度に描画する最大数
    static constexpr int MAX_INSTANCE = 1024;
    //頂点バッファ数
    static constexpr int NUM_VERTEX_BUFFER = 2;
};
