#include "LineRenderer.h"
#include "../../../../DirectX/DirectXInclude.h"
#include "../../../../System/AssetsManager.h"
#include "../../../../System/GlobalFunction.h"
#include "../../../../System/Shader/ShaderBinder.h"

LineRenderer::LineRenderer(DimensionType type)
    : mDimensionType(type)
    , mShaderID(-1)
{
}

LineRenderer::~LineRenderer() = default;

void LineRenderer::initialize() {
    //バーテックスバッファの作成
    if (!mVertexBuffer2D || !mVertexBuffer3D) {
        createVertexBuffer();
    }
    //インデックスバッファの作成
    if (!mIndexBuffer) {
        createIndexBuffer();
    }

    //シェーダーを作成する
    mShaderID = AssetsManager::instance().createShader(getShaderName());
}

void LineRenderer::draw(const Matrix4& proj) {
    //描画共通処理は最初に済ませる

    //バーテックスバッファーを登録
    VertexBuffer* vb = nullptr;
    if (mDimensionType == DimensionType::TWO) {
        vb = mVertexBuffer2D;
    } else if (mDimensionType == DimensionType::THREE) {
        vb = mVertexBuffer3D;
    }
    vb->setVertexBuffer();

    //インデックスバッファーを登録
    mIndexBuffer->setIndexBuffer();

    //シェーダーを登録
    ShaderBinder::bind(mShaderID);

    //子クラスの描画前処理を呼び出す
    drawingPreprocessing(proj);

    //子クラスに描画指令を出す
    drawLines(proj);
}

void LineRenderer::finalize() {
    safeDelete(mIndexBuffer);
    safeDelete(mVertexBuffer3D);
    safeDelete(mVertexBuffer2D);
}

void LineRenderer::createVertexBuffer() {
    //それぞれ始点と終点
    const Vector2 vert2D[] = { Vector2::zero, Vector2::one };
    const Vector3 vert3D[] = { Vector3::zero, Vector3::one };

    BufferDesc bd;
    bd.oneSize = sizeof(Vector2);
    bd.size = bd.oneSize * 2; //頂点が2つだから
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);
    SubResourceDesc sub;
    sub.data = vert2D;
    mVertexBuffer2D = new VertexBuffer(bd, &sub);

    bd.oneSize = sizeof(Vector3);
    bd.size = bd.oneSize * 2; //頂点が2つだから
    sub.data = vert3D;
    mVertexBuffer3D = new VertexBuffer(bd, &sub);
}

void LineRenderer::createIndexBuffer() {
    static constexpr unsigned short indices[] = {
        0, 1
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::INDEX);

    SubResourceDesc sub;
    sub.data = indices;

    mIndexBuffer = new IndexBuffer(bd, sub);
}
