#include "Texture.h"
#include "Format/ITextureReader.h"
#include "Format/PNG/PngReader.h"
#include "../GlobalFunction.h"
#include "../Shader/Shader.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../Utility/FileUtil.h"

Texture::Texture()
    : mShaderResourceView(nullptr)
    , mSampler(nullptr)
    , mWidth(0)
    , mHeight(0)
{
    if (!vertexBuffer || !indexBuffer) {
        //バーテックスバッファー作成
        createVertexBuffer();
        //インデックスバッファの作成
        createIndexBuffer();
    }
    //テクスチャー用サンプラー作成
    createSampler();
}

Texture::Texture(const std::string& filePath)
    : Texture()
{
    createTextureFromFilePath(filePath);
}

Texture::Texture(unsigned width, unsigned height)
    : Texture()
{
    mColors.resize(width * height * PIXEL_DATA_SIZE, 255);
    createTextureFromMemory(width, height);
}

Texture::Texture(const std::vector<unsigned char>& data, unsigned width, unsigned height)
    : Texture()
{
    mColors.resize(width * height * PIXEL_DATA_SIZE);
    std::copy(data.begin(), data.end(), mColors.begin());
    createTextureFromMemory(width, height);
}

Texture::Texture(const std::shared_ptr<ShaderResourceView>& view, unsigned width, unsigned height)
    : mShaderResourceView(view)
    , mSampler(nullptr)
    , mWidth(width)
    , mHeight(height)
{
    if (!vertexBuffer || !indexBuffer) {
        //バーテックスバッファー作成
        createVertexBuffer();
        //インデックスバッファの作成
        createIndexBuffer();
    }
    //テクスチャー用サンプラー作成
    createSampler();
}

Texture::~Texture() = default;

void Texture::clear() {
    std::fill(mColors.begin(), mColors.end(), 255);
}

void Texture::setPixel(unsigned x, unsigned y, const Vector3& color) {
    setPixel(x, y, color.x * 255, color.y * 255, color.z * 255);
}

void Texture::setPixel(unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b) {
    //データがunsigned char型で色3つアルファ値1つ並んでるから
    auto p = y * mWidth * PIXEL_DATA_SIZE + x * PIXEL_DATA_SIZE;
    mColors[p] = r;
    mColors[p + 1] = g;
    mColors[p + 2] = b;
}

void Texture::apply() {
    createTextureFromMemory(mWidth, mHeight);
}

unsigned Texture::getWidth() const {
    return mWidth;
}

unsigned Texture::getHeight() const {
    return mHeight;
}

const ShaderResourceView& Texture::getShaderResourceView() const {
    return *mShaderResourceView;
}

const Sampler& Texture::getSampler() const {
    return *mSampler;
}

void Texture::finalize() {
    safeDelete(vertexBuffer);
    safeDelete(vertexBuffer3D);
    safeDelete(indexBuffer);
}

void Texture::createVertexBuffer() {
    static const TextureVertex vertices[] = {
        Vector3(0.f, 0.f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(1.f, 0.f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(0.f, 1.f, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(1.f, 1.f, 0.f), Vector2(1.f, 1.f), //右下
    };

    BufferDesc bd;
    bd.oneSize = sizeof(TextureVertex);
    bd.size = bd.oneSize * 4; //頂点が4つだから
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);

    SubResourceDesc sub;
    sub.data = vertices;

    vertexBuffer = new VertexBuffer(bd, &sub);

    static const TextureVertex vertices3D[] = {
        Vector3(-0.5f, 0.5f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(0.5f, 0.5f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(-0.5f, -0.5f, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(0.5f, -0.5f, 0.f), Vector2(1.f, 1.f), //右下
    };

    sub.data = vertices3D;

    vertexBuffer3D = new VertexBuffer(bd, &sub);
}

void Texture::createIndexBuffer() {
    static constexpr unsigned short indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::INDEX);

    SubResourceDesc sub;
    sub.data = indices;
    indexBuffer = new IndexBuffer(bd, sub);
}

void Texture::createSampler() {
    SamplerDesc sd;
    mSampler = std::make_unique<Sampler>(sd);
}

void Texture::createTextureFromFilePath(const std::string& filePath) {
    //拡張子によって処理を分ける
    const auto& ext = FileUtil::getFileExtension(filePath);
    std::unique_ptr<ITextureReader> reader = nullptr;
    if (ext == ".png") {
        reader = std::make_unique<PngReader>();
    } else {
        Debug::windowMessage(filePath + ": 対応していない拡張子です");
        return;
    }

    //メッシュを解析する
    reader->read(filePath, mColors);

    //テクスチャを作成する
    createTextureFromMemory(reader->getWidth(), reader->getHeight());
}

void Texture::createTextureFromMemory(unsigned width, unsigned height) {
    mWidth = width;
    mHeight = height;

    Texture2DDesc tex2DDesc{};
    tex2DDesc.width = width;
    tex2DDesc.height = height;

    SubResourceDesc sub{};
    sub.data = mColors.data();
    sub.pitch = width * PIXEL_DATA_SIZE;

    auto tex2D = std::make_unique<Texture2D>(tex2DDesc, &sub);

    mShaderResourceView = std::make_shared<ShaderResourceView>(*tex2D);
}
