#pragma once

#include "../../Device/FileOperator.h"
#include "../../Math/Math.h"

class ModelViewerLight
    : public FileOperator
{
public:
    ModelViewerLight();
    ~ModelViewerLight();
    void drawGUI();
    Vector3 getDirection() const;
    const Vector3& getColor() const;

private:
    ModelViewerLight(const ModelViewerLight&) = delete;
    ModelViewerLight& operator=(const ModelViewerLight&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    Quaternion mDirection;
    Vector3 mColor;
    Vector3 mDirectionDrawPosition;
    float mLengthDirection;
};
