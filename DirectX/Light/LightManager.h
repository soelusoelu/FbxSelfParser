﻿#pragma once

#include "../Math/Math.h"
#include "../Device/FileOperator.h"
#include <list>
#include <memory>

class DirectionalLight;
class PointLightComponent;
class Camera;
struct PointLight;

class LightManager
    : public FileOperator
{
    using PointLightPtr = std::shared_ptr<PointLightComponent>;
    using PointLightPtrList = std::list<PointLightPtr>;

public:
    LightManager();
    ~LightManager();
    LightManager(const LightManager&) = delete;
    LightManager& operator=(const LightManager&) = delete;

    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;
    virtual void childSaveAndLoad(JsonObject& inObj, FileMode mode) override;

    void initialize();
    void createDirectionalLight();
    //ディレクショナルライト
    const DirectionalLight& getDirectionalLight() const;
    //アンビエントライト
    void setAmbientLight(const Vector3& ambient);
    const Vector3& getAmbientLight() const;
    //ポイントライト
    void addPointLight(const PointLightPtr& pointLight);
    void removePointLight(const PointLightPtr& pointLight);
    void drawPointLights(const Camera& camera);

private:
    Vector3 mAmbientLight;
    std::shared_ptr<DirectionalLight> mDirectionalLight;
    PointLightPtrList mPointLights;
    std::unique_ptr<PointLight> mPointLight;
};
