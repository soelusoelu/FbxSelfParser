#include "PointLight.h"
#include "../DirectX/InputElement.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
#include "../System/AssetsManager.h"
#include "../System/Shader/Shader.h"
#include "../Utility/JsonHelper.h"

PointLight::PointLight()
    : FileOperator("PointLight")
    , shader(nullptr)
    , radius(0.f)
    , mMeshFileName()
{
}

PointLight::~PointLight() = default;

void PointLight::saveAndLoad(JsonObject & inObj, FileMode mode) {
    JsonHelper::getSet(mMeshFileName, "pointLightMeshFileName", inObj, mode);
}

void PointLight::initialize() {
    //mesh = World::instance().assetsManager().createMeshLoader(mMeshFileName);
    //mesh->setInitMaterials(&materials);
    //radius = mesh->getRadius();
    //shader = World::instance().assetsManager().createShader("PointLight.hlsl");
}
