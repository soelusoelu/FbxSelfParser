#include "LineRenderer3D.h"
#include "../../../../DirectX/DirectXInclude.h"
#include "../../../../System/Shader/ConstantBuffers.h"
#include "../../../../System/Shader/DataTransfer.h"

LineRenderer3D::LineRenderer3D()
    : LineRenderer(DimensionType::THREE)
{
}

LineRenderer3D::~LineRenderer3D() = default;

void LineRenderer3D::clear() {
    mLines.clear();
}

void LineRenderer3D::renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color) {
    mLines.emplace_back(Line3DParam{ p1, p2, color });
}

std::string LineRenderer3D::getShaderName() {
    return "Line3D.hlsl";
}

void LineRenderer3D::drawLines(const Matrix4& proj) const {
    for (const auto& line : mLines) {
        drawLine(line, proj);
    }
}

void LineRenderer3D::drawLine(const Line3DParam& param, const Matrix4& proj) const {
    //パラメータからワールド行列を計算する
    auto mat = Matrix4::createScale(param.p2 - param.p1);
    mat *= Matrix4::createTranslation(param.p1);

    //シェーダーに値を渡す
    LineConstantBuffer cb;
    cb.wp = mat * proj;
    cb.color = Vector4(param.color, 1.f);

    //シェーダーにデータ転送
    DataTransfer::transferConstantBuffer(mShaderID, &cb);

    //描画
    MyDirectX::DirectX::instance().drawIndexed(2);
}
