#include "MeshManager.h"
#include "Shadow/ShadowMap.h"
#include "../Component/Engine/Mesh/MeshComponent.h"
#include "../Component/Engine/Mesh/MeshRenderer.h"
#include "../DirectX/DirectXInclude.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Transform/Transform3D.h"

MeshManager::MeshManager()
    : mShadowMap(std::make_unique<ShadowMap>())
{
    MeshRenderer::setMeshManager(this);
}

MeshManager::~MeshManager() {
    MeshRenderer::setMeshManager(nullptr);
}

const MeshPtrList& MeshManager::getMeshes() const {
    return mShadowMeshes;
}

void MeshManager::add(const MeshPtr& mesh, bool handleShadow) {
    if (handleShadow) {
        mShadowMeshes.emplace_back(mesh);
    } else {
        mMeshes.emplace_back(mesh);
    }
}

void MeshManager::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mShadowMap->writeAndRead(inObj, alloc, mode);
}

void MeshManager::initialize() {
    mShadowMap->initialize();
}

void MeshManager::update() {
    remove();
}

void MeshManager::draw(
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    if (mShadowMeshes.empty() && mMeshes.empty()) {
        return;
    }

    if (mShadowMap) {
        drawShadow(view, projection, cameraPosition, dirLightDirection, dirLightColor);
    }

    drawMeshes(view, projection, cameraPosition, dirLightDirection, dirLightColor);

    //ワイヤーフレームになっているかも知れないからソリッドに戻す
    MyDirectX::DirectX::instance().rasterizerState().setFillMode(FillMode::SOLID);
}

void MeshManager::clear() {
    remove();
}

void MeshManager::erase(const MeshPtr& mesh) {
    mShadowMeshes.remove(mesh);
    mMeshes.remove(mesh);
}

void MeshManager::registerThisToMeshRenderer() {
    MeshRenderer::setMeshManager(this);
}

void MeshManager::remove() {
    auto itr = mShadowMeshes.begin();
    while (itr != mShadowMeshes.end()) {
        if ((*itr)->getMeshComponent().isDead()) {
            itr = mShadowMeshes.erase(itr);
        } else {
            ++itr;
        }
    }

    auto itr2 = mMeshes.begin();
    while (itr2 != mMeshes.end()) {
        if ((*itr2)->getMeshComponent().isDead()) {
            itr2 = mMeshes.erase(itr2);
        } else {
            ++itr2;
        }
    }
}

bool MeshManager::isDraw(const MeshRenderer& mesh) const {
    if (mesh.isDead()) {
        return false;
    }
    //auto pos = mesh.transform().getPosition();
    //auto radius = mesh.getMesh().getRadius() * mesh.transform().getScale().x;
    //if (!camera.viewFrustumCulling(pos, radius)) {
    //    return false;
    //}

    return true;
}

void MeshManager::drawMeshes(
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    //影の影響を受けるメッシュの描画
    for (const auto& mesh : mShadowMeshes) {
        if (!isDraw(*mesh)) {
            continue;
        }

        //深度テクスチャを転送する
        mShadowMap->transferShadowTexture();

        mesh->draw(view, projection, cameraPosition, dirLightDirection, dirLightColor);

        //深度テクスチャの後処理
        mShadowMap->drawEndShadowTexture();
    }

    //影の影響を受けないメッシュの描画
    for (const auto& mesh : mMeshes) {
        if (!isDraw(*mesh)) {
            continue;
        }

        mesh->draw(view, projection, cameraPosition, dirLightDirection, dirLightColor);
    }
}

void MeshManager::drawShadow(
    const Matrix4& view,
    const Matrix4& projection,
    const Vector3& cameraPosition,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    if (mShadowMeshes.empty()) {
        return;
    }

    //描画準備
    mShadowMap->drawBegin(dirLightDirection);

    for (const auto& mesh : mShadowMeshes) {
        //描画できないなら次へ
        if (!isDraw(*mesh)) {
            continue;
        }

        //描画
        mShadowMap->draw(*mesh);

        //影描画用の定数バッファを設定する
        mShadowMap->setShadowConstantBuffer(*mesh);
    }

    //描画終了処理
    mShadowMap->drawEnd();
}
