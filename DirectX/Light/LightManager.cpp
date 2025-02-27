﻿#include "LightManager.h"
#include "PointLight.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Component/Engine/Light/PointLightComponent.h"
#include "../DirectX/DirectXInclude.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Mesh/IMeshLoader.h"
#include "../System/SystemInclude.h"
#include "../System/Shader/Shader.h"
#include "../Utility/JsonHelper.h"

LightManager::LightManager()
    : FileOperator("LightManager")
    , mAmbientLight(Vector3::zero)
    , mPointLight(std::make_unique<PointLight>())
{
    PointLightComponent::setLightManager(this);
}

LightManager::~LightManager() {
    PointLightComponent::setLightManager(nullptr);
}

void LightManager::saveAndLoad(JsonObject& inObj, FileMode mode) {
    JsonHelper::getSet(mAmbientLight, "ambientLight", inObj, mode);
}

void LightManager::childSaveAndLoad(JsonObject& inObj, FileMode mode) {
    mPointLight->writeAndRead(inObj, mode);
}

void LightManager::initialize() {
    mPointLight->initialize();
}

void LightManager::createDirectionalLight() {
    auto dirLight = GameObjectCreater::create("DirectionalLight");
    mDirectionalLight = dirLight->componentManager().getComponent<DirectionalLight>();
    //向き計算のために
    mDirectionalLight->lateUpdate();
}

const DirectionalLight& LightManager::getDirectionalLight() const {
    return *mDirectionalLight;
}

void LightManager::setAmbientLight(const Vector3& ambient) {
    mAmbientLight = ambient;
}

const Vector3& LightManager::getAmbientLight() const {
    return mAmbientLight;
}

void LightManager::addPointLight(const PointLightPtr& pointLight) {
    mPointLights.emplace_back(pointLight);
}

void LightManager::removePointLight(const PointLightPtr& pointLight) {
    mPointLights.remove(pointLight);
}

void LightManager::drawPointLights(const Camera& camera) {
    //if (mPointLights.empty()) {
    //    return;
    //}

    //auto& dx = DirectX::instance();
    //auto shader = mPointLight->shader;
    ////使用するシェーダーの登録
    //shader->setVSShader();
    //shader->setPSShader();
    ////このコンスタントバッファーを使うシェーダーの登録
    //shader->setVSConstantBuffers();
    //shader->setPSConstantBuffers();
    ////頂点インプットレイアウトをセット
    //shader->setInputLayout();
    ////バーテックスバッファーをセット
    ////mPointLight->mesh->getVertexArray()->setVertexBuffer();
    ////プリミティブ指定
    //dx.setPrimitive(PrimitiveType::TRIANGLE_LIST);
    ////デプステスト有効化
    //dx.depthStencilState()->depthTest(true);
    ////デプスマスク無効化
    //dx.depthStencilState()->depthMask(false);
    ////加算合成
    //dx.blendState()->add();

    //for (const auto& pointLight : mPointLights) {
    //    pointLight->draw(camera, *mPointLight);
    //}
}
