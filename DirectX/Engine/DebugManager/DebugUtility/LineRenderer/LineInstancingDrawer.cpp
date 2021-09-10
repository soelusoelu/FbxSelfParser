#include "LineInstancingDrawer.h"
#include "LineRenderer3D.h"
#include "../../../../System/AssetsManager.h"
#include "../../../../System/Shader/DataTransfer.h"
#include "../../../../System/Shader/ShaderBinder.h"
#include "../../../../Transform/Transform3D.h"

LineInstancingDrawer::LineInstancingDrawer()
    : LineRenderer(DimensionType::THREE)
    , mInputBuffer(nullptr)
{
}

LineInstancingDrawer::~LineInstancingDrawer() = default;

void LineInstancingDrawer::initialize() {
    LineRenderer::initialize();

    //入力バッファの作成
    BufferDesc bd{};
    bd.size = sizeof(LineConstantBuffer) * MAX_INSTANCE;
    bd.usage = Usage::DYNAMIC;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);
    bd.cpuAccessFlags = static_cast<unsigned>(BufferCPUAccessFlag::WRITE);

    mInputBuffer = std::make_unique<VertexBuffer>(bd);
}

void LineInstancingDrawer::add(const Vector3& p1, const Vector3& p2, const Vector3& color) {
    auto mat = Matrix4::createScale(p2 - p1);
    mat *= Matrix4::createTranslation(p1);

    //スプライトから描画に必要なデータを抜き出す
    LineConstantBuffer lcb{};
    lcb.wp = mat;
    lcb.color = Vector4(color, 1.f);

    //新しいインスタンスデータを登録
    mInstancingData.emplace_back(lcb);
}

void LineInstancingDrawer::clear() {
    mInstancingData.clear();
}

std::string LineInstancingDrawer::getShaderName() {
    return "Line3DInstancing.hlsl";
}

void LineInstancingDrawer::drawingPreprocessing(const Matrix4& viewProj) {
    //全ての描画データに射影行列を掛ける
    for (auto&& data : mInstancingData) {
        data.wp *= viewProj;
    }
}

void LineInstancingDrawer::drawLines(const Matrix4& viewProj) const {
    if (mInstancingData.empty()) {
        return;
    }

    //インスタンシング用頂点バッファを登録する
    ID3D11Buffer* buffers[NUM_VERTEX_BUFFER] = { mVertexBuffer3D->buffer(), mInputBuffer->buffer() };
    unsigned strides[NUM_VERTEX_BUFFER] = { mVertexBuffer3D->desc().oneSize, sizeof(LineConstantBuffer) };
    mVertexBuffer3D->setVertexBuffer(NUM_VERTEX_BUFFER, buffers, strides);

    //シェーダーにデータ転送
    DataTransfer::transferData(
        *mInputBuffer,
        mInstancingData.data(),
        mInstancingData.size()
    );

    //描画
    MyDirectX::DirectX::instance().drawIndexedInstanced(2, mInstancingData.size());
}
