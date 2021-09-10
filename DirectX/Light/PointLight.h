#pragma once

#include "../Device/FileOperator.h"
#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
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
    void initialize();

private:
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::string mMeshFileName;
};
