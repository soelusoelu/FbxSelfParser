#pragma once

#include "../../Component.h"
#include "../../../Mesh/IAnimation.h"
#include "../../../Mesh/IMesh.h"
#include <memory>
#include <unordered_map>
#include <utility>

//メッシュのシェーダーを扱うクラス
class MeshShader
    : public Component
{
public:
    MeshShader();
    ~MeshShader();
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    //シェーダーをバインドする
    void bindShader() const;
    //シェーダーにデータを転送する
    void transferData();

    //メッシュ共通の値を設定する
    void setCommonValue(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;
    //マテリアルデータを設定する
    void setMaterialData(unsigned materialIndex, unsigned constantBufferIndex = 1) const;
    //全メッシュに共通する値を設定する
    //データが生きている必要あり
    void setTransferData(const void* data, unsigned constantBufferIndex);

    //各種インターフェースを設定する
    void setInterface(const IMesh* mesh, const IAnimation* anim);
    //デフォルトのシェーダーに変更する
    void setDefaultShader();

private:
    MeshShader(const MeshShader&) = delete;
    MeshShader& operator=(const MeshShader&) = delete;

private:
    const IMesh* mMesh;
    const IAnimation* mAnimation;
    int mShaderID;
    std::unordered_map<unsigned, const void*> mTransferDataMap;
};
