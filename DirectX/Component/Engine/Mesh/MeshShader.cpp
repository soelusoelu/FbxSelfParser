#include "MeshShader.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Imgui/imgui.h"
#include "../../../Mesh/Material.h"
#include "../../../Mesh/MeshCommonShaderSetter.h"
#include "../../../System/AssetsManager.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include "../../../System/Shader/DataTransfer.h"
#include "../../../System/Shader/Shader.h"
#include "../../../System/Shader/ShaderBinder.h"
#include "../../../System/Texture/Texture.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

MeshShader::MeshShader()
    : Component()
    , mMesh(nullptr)
    , mAnimation(nullptr)
    , mShaderID(-1)
{
}

MeshShader::~MeshShader() = default;

void MeshShader::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::SAVE) {
        JsonHelper::setString(AssetsManager::instance().getShaderFormID(mShaderID).getShaderName(), "shaderName", inObj, alloc);
    } else {
        //シェーダー名が取得できたら読み込む
        if (std::string shader; JsonHelper::getString(shader, "shaderName", inObj)) {
            //シェーダーを生成する
            mShaderID = AssetsManager::instance().createShader(shader);
        } else {
            //できなかったらデフォルトを使う
            setDefaultShader();
        }
    }
}

void MeshShader::drawInspector() {
    ImGui::Text("Shader: %s", AssetsManager::instance().getShaderFormID(mShaderID).getShaderName().c_str());
}

void MeshShader::bindShader() const {
    //使用するシェーダーの登録
    ShaderBinder::bind(mShaderID);
}

void MeshShader::transferData() {
    //何も登録されてないなら終了
    if (mTransferDataMap.empty()) {
        return;
    }

    //すべてのデータを転送する
    for (const auto& transferData : mTransferDataMap) {
        DataTransfer::transferConstantBuffer(mShaderID, transferData.second, transferData.first);
    }
}

void MeshShader::setCommonValue(
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    MeshCommonConstantBuffer meshcb{};
    MeshCommonShaderSetter::setCommon(meshcb, transform().getWorldTransform(), view, projection, cameraPosition, dirLightDirection, dirLightColor);
    DataTransfer::transferConstantBuffer(mShaderID, &meshcb, 0);
}

void MeshShader::setMaterialData(unsigned materialIndex, unsigned constantBufferIndex) const {
    MaterialConstantBuffer matcb{};
    MeshCommonShaderSetter::setMaterial(matcb, mMesh->getMaterial(materialIndex));
    DataTransfer::transferConstantBuffer(mShaderID, &matcb, constantBufferIndex);
}

void MeshShader::setTransferData(const void* data, unsigned constantBufferIndex) {
    mTransferDataMap[constantBufferIndex] = data;
}

void MeshShader::setInterface(const IMesh* mesh, const IAnimation* anim) {
    mMesh = mesh;
    mAnimation = anim;
    setDefaultShader();
}

void MeshShader::setDefaultShader() {
    std::string shader;
    //影の影響を受けるか
    if (getComponent<MeshComponent>()->handleShadow()) {
        shader = "Shadow.hlsl";
        //ボーンが有るなら
        //if (mAnimation->getBoneCount() > 0) {
        //    shader = "SkinMeshShadow.hlsl";
        //}
    } else {
        shader = "Mesh.hlsl";
        //ボーンが有るなら
        //if (mAnimation->getBoneCount() > 0) {
        //    shader = "SkinMesh.hlsl";
        //}
    }

    //シェーダーを生成する
    mShaderID = AssetsManager::instance().createShader(shader);
}
