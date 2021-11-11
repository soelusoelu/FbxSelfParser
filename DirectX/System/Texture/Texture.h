#pragma once

#include "../../Math/Math.h"
#include <memory>
#include <string>
#include <vector>

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
    //ファイルパスからテクスチャを作成する
    Texture(const std::string& filePath);
    //入力された横幅 * 縦幅の白テクスチャを作成する
    Texture(unsigned width, unsigned height);
    //入力されたデータからテクスチャを作成する
    Texture(const std::vector<unsigned char>& data, unsigned width, unsigned height);
    //シェーダーリソースビューを受け取りこのクラスで管理する
    Texture(const std::shared_ptr<ShaderResourceView>& view, const Vector2& textureSize);
    virtual ~Texture();

    //テクスチャを白色で塗りつぶす
    void clear();
    //指定した座標の色を設定する [0, 1]
    void setPixel(unsigned x, unsigned y, const Vector3& color);
    //指定した座標の色を設定する [0, 255]
    void setPixel(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b);
    //clear, setPixel関数での変更を適用する
    void apply();

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
    //ファイル名からテクスチャを作成する
    void createTextureFromFileName(const std::string& filePath);
    //テクスチャを作成する
    void createTextureFromMemory(unsigned width, unsigned height);

public:
    static inline VertexBuffer* vertexBuffer = nullptr;
    static inline VertexBuffer* vertexBuffer3D = nullptr;
    static inline IndexBuffer* indexBuffer = nullptr;

protected:
    std::shared_ptr<ShaderResourceView> mShaderResourceView;
    std::unique_ptr<Sampler> mSampler;
    //RGBA 8bit
    std::vector<unsigned char> mColors;
    Vector2 mTextureSize;

    //1ピクセルのデータサイズ(RGBA各1バイト)
    static constexpr int PIXEL_DATA_SIZE = sizeof(unsigned char) * 4;
};
