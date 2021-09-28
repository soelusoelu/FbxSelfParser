#pragma once

#include "../../Device/FileOperator.h"
#include "../../Math/Math.h"

class ModelViewerLight
    : public FileOperator
{
public:
    ModelViewerLight();
    ~ModelViewerLight();
    ModelViewerLight(const ModelViewerLight&) = delete;
    ModelViewerLight& operator=(const ModelViewerLight&) = delete;

    virtual void saveAndLoad(JsonObject& inObj, FileMode mode) override;

    void drawGUI();
    Vector3 getDirection() const;
    const Vector3& getColor() const;

private:
    Quaternion mDirection;
    Vector3 mColor;
    Vector3 mDirectionDrawPosition;
    float mLengthDirection;
};
