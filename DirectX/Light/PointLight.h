#pragma once

#include "../Device/FileOperator.h"
#include "../Math/Math.h"
#include <memory>
#include <string>
#include <vector>

class IMeshLoader;
class Shader;
struct Material;

struct PointLight
    : public FileOperator
{
    std::shared_ptr<IMeshLoader> mesh;
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Shader> shader;
    float radius;

    PointLight();
    ~PointLight();
    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;
    void initialize();

private:
    std::string mMeshFileName;
};
