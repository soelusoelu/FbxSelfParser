#pragma once

#include "IDrawBefore.h"
#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IMesh.h"
#include "../../../Mesh/IMeshDrawer.h"
#include <memory>

class SkinMeshComponent;
class Camera;
class DirectionalLight;

class MeshOutLine : public Component, public IDrawBefore {
public:
    MeshOutLine();
    ~MeshOutLine();
    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    //メッシュ描画前描画
    virtual void drawBefore(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const override;

    //アウトラインの色を設定する
    void setOutLineColor(const Vector3& color);
    //アウトラインの色を取得する
    const Vector3& getOutLineColor() const;
    //アウトラインの太さを設定する
    void setOutLineThickness(float thickness);
    //アウトラインの太さを取得する
    float getOutLineThickness() const;
    //アウトラインを描画するか設定する
    void setActiveOutLine(bool value);
    //アウトラインを描画するか取得する
    bool getActiveOutLine() const;

private:
    MeshOutLine(const MeshOutLine&) = delete;
    MeshOutLine& operator=(const MeshOutLine&) = delete;

    //アウトラインを描画する
    void drawOutLine(const Matrix4& view, const Matrix4& projection) const;

protected:
    const IMesh* mMesh;
    const IMeshDrawer* mDrawer;
    int mOutLineShaderID;
    std::shared_ptr<SkinMeshComponent> mSkinMesh;
    Vector3 mOutLineColor;
    float mOutLineThickness;
    bool mIsDrawOutLine;
    bool mIsAnimation;
    float mOffset;
};
