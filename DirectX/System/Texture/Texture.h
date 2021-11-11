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
    Texture(const std::shared_ptr<ShaderResourceView>& view, unsigned width, unsigned height);
    ~Texture();
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    //テクスチャを白色で塗りつぶす
    void clear();
    //指定した座標の色を設定する [0, 1]
    void setPixel(unsigned x, unsigned y, const Vector3& color);
    //指定した座標の色を設定する [0, 255]
    void setPixel(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b);
    //clear, setPixel関数での変更を適用する
    void apply();

    //テクスチャの横幅を取得する
    unsigned getWidth() const;
    //テクスチャの縦幅を取得する
    unsigned getHeight() const;
    //シェーダーリソースビューを取得する
    const ShaderResourceView& getShaderResourceView() const;
    //サンプラーを取得する
    const Sampler& getSampler() const;

    //終了処理
    static void finalize();

private:
    void createVertexBuffer();
    void createIndexBuffer();
    void createSampler();
    //ファイル名からテクスチャを作成する
    void createTextureFromFilePath(const std::string& filePath);
    //テクスチャを作成する
    void createTextureFromMemory(unsigned width, unsigned height);

public:
    static inline VertexBuffer* vertexBuffer = nullptr;
    static inline VertexBuffer* vertexBuffer3D = nullptr;
    static inline IndexBuffer* indexBuffer = nullptr;

private:
    std::shared_ptr<ShaderResourceView> mShaderResourceView;
    std::unique_ptr<Sampler> mSampler;
    //RGBA 8bit
    std::vector<unsigned char> mColors;
    //テクスチャ横幅
    unsigned mWidth;
    //テクスチャ縦幅
    unsigned mHeight;

    //1ピクセルのデータサイズ(RGBA各1バイト)
    static constexpr int PIXEL_DATA_SIZE = sizeof(unsigned char) * 4;
};
