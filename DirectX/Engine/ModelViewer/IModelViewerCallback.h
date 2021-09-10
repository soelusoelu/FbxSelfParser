#pragma once

#include "ModelViewerMode.h"
#include <functional>

class GameObject;

class IModelViewerCallback {
public:
    virtual ~IModelViewerCallback() = default;
    virtual void callbackModeChange(const std::function<void(ModelViewerMode)>& f) = 0;
    virtual void callbackModelChange(const std::function<void(GameObject&)>& f) = 0;
};
