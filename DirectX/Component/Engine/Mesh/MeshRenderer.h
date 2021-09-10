#pragma once

#include "IDrawBefore.h"
#include "../../Component.h"
#include "../../../DirectX/DirectXInclude.h"
#include "../../../Math/Math.h"
#include <memory>

class MeshComponent;
class MeshShader;
class MeshManager;

//メッシュ描画専用クラス
class MeshRenderer
    : public Component
    , public std::enable_shared_from_this<MeshRenderer>
{
public:
    MeshRenderer();
    ~MeshRenderer();
    virtual void start() override;

    //描画
    void draw(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;
    //描画前描画をしたいクラスを登録する
    void setDrawBefore(const IDrawBefore* drawer);
    //ポリゴン描画方法を設定する
    void setFillMode(FillMode mode);
    //ポリゴン描画方法を取得する
    FillMode getFillMode() const;
    //メッシュコンポーネントを取得する
    const MeshComponent& getMeshComponent() const;
    //メッシュシェーダーを取得する
    MeshShader& getMeshShader() const;

    //自身を管理するマネージャーを登録する
    static void setMeshManager(MeshManager* manager);

private:
    MeshRenderer(const MeshRenderer&) = delete;
    MeshRenderer& operator=(const MeshRenderer&) = delete;

    //メインメッシュ描画
    void drawMesh(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;
    //スキンメッシュコンポーネントを条件に合えば作る
    void createSkinMeshComponent();
    //マネージャーに登録する
    void addToManager();

private:
    const IDrawBefore* mBeforeDrawer;
    std::shared_ptr<MeshComponent> mMeshComponent;
    std::shared_ptr<MeshShader> mMeshShader;
    FillMode mFillMode;

    static inline MeshManager* mMeshManager = nullptr;
};
