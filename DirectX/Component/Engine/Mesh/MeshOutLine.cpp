#include "MeshOutLine.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "SkinMeshComponent.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../../DirectX/DirectXInclude.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../Imgui/imgui.h"
#include "../../../Mesh/Material.h"
#include "../../../Mesh/Mesh.h"
#include "../../../System/AssetsManager.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include "../../../System/Shader/DataTransfer.h"
#include "../../../System/Shader/ShaderBinder.h"
#include "../../../System/Texture/TextureFromFile.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

MeshOutLine::MeshOutLine()
    : Component()
    , mMesh(nullptr)
    , mDrawer(nullptr)
    , mOutLineShaderID(-1)
    , mSkinMesh(nullptr)
    , mOutLineColor(ColorPalette::white)
    , mOutLineThickness(0.1f)
    , mIsDrawOutLine(true)
    , mIsAnimation(false)
    , mOffset(0.f)
{
}

MeshOutLine::~MeshOutLine() = default;

void MeshOutLine::start() {
    auto mesh = getComponent<MeshComponent>();

    //描画インターフェース取得
    mMesh = mesh->getMesh();
    mDrawer = mesh->getDrawer();

    //ボーンの有無でアニメーションするモデルか判断する
    mIsAnimation = (mesh->getAnimation()->getBoneCount() > 0);

    //アニメーションするかでシェーダーを決める
    auto name = (mIsAnimation) ? "SkinMeshOutLine.hlsl" : "OutLine.hlsl";
    mOutLineShaderID = AssetsManager::instance().createShader(name);

    if (mIsAnimation) {
        mSkinMesh = getComponent<SkinMeshComponent>();
    }

    //描画前描画の登録
    getComponent<MeshRenderer>()->setDrawBefore(this);
}

void MeshOutLine::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mOutLineColor, "outLineColor", inObj, alloc, mode);
    JsonHelper::getSet(mOutLineThickness, "outLineColorThickness", inObj, alloc, mode);
    JsonHelper::getSet(mIsDrawOutLine, "isDrawOutLine", inObj, alloc, mode);
    JsonHelper::getSet(mOffset, "offset", inObj, alloc, mode);
}

void MeshOutLine::drawInspector() {
    ImGuiWrapper::colorEdit3("OutLineColor", mOutLineColor);
    ImGui::SliderFloat("OutLineThickness", &mOutLineThickness, 0.f, 1.f);
    ImGui::Checkbox("IsDrawOutLine", &mIsDrawOutLine);
    ImGuiWrapper::dragFloat("Offset", mOffset, 0.01f);
}

void MeshOutLine::drawBefore(
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    if (mIsDrawOutLine) {
        //裏面のみ描画したいから
        auto& rs = MyDirectX::DirectX::instance().rasterizerState();
        rs.setCulling(CullMode::FRONT);

        //アウトライン描画
        drawOutLine(view, projection);

        //設定を戻す
        rs.setCulling(CullMode::BACK);
    }
}

void MeshOutLine::setOutLineColor(const Vector3& color) {
    mOutLineColor = color;
}

const Vector3& MeshOutLine::getOutLineColor() const {
    return mOutLineColor;
}

void MeshOutLine::setOutLineThickness(float thickness) {
    mOutLineThickness = thickness;
}

float MeshOutLine::getOutLineThickness() const {
    return mOutLineThickness;
}

void MeshOutLine::setActiveOutLine(bool value) {
    mIsDrawOutLine = value;
}

bool MeshOutLine::getActiveOutLine() const {
    return mIsDrawOutLine;
}

void MeshOutLine::drawOutLine(const Matrix4& view, const Matrix4& projection) const {
    //アウトラインシェーダーの登録
    ShaderBinder::bind(mOutLineShaderID);

    //スケールを拡大したモデルをアウトラインとして描画するため
    //ワールド行列の再計算をする
    const auto& t = transform();
    auto offset = Vector3::up * mOffset;
    auto world = Matrix4::createTranslation(-t.getPivot() - offset);
    world *= Matrix4::createScale(t.getScale() + Vector3::one * mOutLineThickness);
    world *= Matrix4::createFromQuaternion(t.getRotation());
    world *= Matrix4::createTranslation(t.getPosition() + offset * t.getScale());

    //シェーダーのコンスタントバッファーに各種データを渡す
    OutLineConstantBuffer outlinecb;
    outlinecb.wvp = world * view * projection;
    outlinecb.outlineColor = Vector4(mOutLineColor, 1.f);
    const auto& shader = AssetsManager::instance().getShaderFormID(mOutLineShaderID);
    DataTransfer::transferConstantBuffer(mOutLineShaderID, &outlinecb, 0);

    //アニメーションするならボーンのデータも渡す
    if (mIsAnimation) {
        DataTransfer::transferConstantBuffer(
            mOutLineShaderID,
            mSkinMesh->getBoneCurrentFrameMatrix().data(),
            1
        );
    }

    //アウトラインを描画する
    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        mDrawer->draw(i);
    }
}
