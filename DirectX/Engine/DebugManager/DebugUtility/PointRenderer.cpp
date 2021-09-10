#include "PointRenderer.h"
#include "../../../DirectX/DirectXInclude.h"
#include "../../../System/AssetsManager.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include "../../../System/Shader/DataTransfer.h"
#include "../../../System/Shader/ShaderBinder.h"
#include "../../../Transform/Transform3D.h"

PointRenderer::PointRenderer() :
    mTransform(std::make_unique<Transform3D>()),
    mShaderID(-1) {
}

PointRenderer::~PointRenderer() = default;

void PointRenderer::draw(const Matrix4& viewProj) const {
    //シェーダーを登録
    ShaderBinder::bind(mShaderID);

    for (const auto& point : mPoints) {
        drawPoint(point, viewProj);
    }
}

void PointRenderer::initialize() {
    //シェーダーを作成する
    mShaderID = AssetsManager::instance().createShader("Point3D.hlsl");
}

void PointRenderer::clear() {
    mPoints.clear();
}

void PointRenderer::renderPoint(const Vector3& point, const Vector3& color) {
    mPoints.emplace_back(Point3DParam{ point, color });
}

void PointRenderer::drawPoint(const Point3DParam& param, const Matrix4& viewProj) const {
    //パラメータからワールド行列を計算する
    mTransform->setPosition(param.point);
    mTransform->computeMatrix();

    //シェーダーに値を渡す
    PointConstantBuffer cb;
    cb.wvp = mTransform->getWorldTransform() * viewProj;
    cb.color = Vector4(param.color, 1.f);

    //シェーダーにデータ転送
    DataTransfer::transferConstantBuffer(mShaderID, &cb);

    //描画
    MyDirectX::DirectX::instance().drawIndexed(1);
}
