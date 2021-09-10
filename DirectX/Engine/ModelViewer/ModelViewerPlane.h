#pragma once

#include "IModelViewerCallback.h"
#include <memory>

class GameObject;
class MeshManager;

class ModelViewerPlane {
public:
    ModelViewerPlane();
    ~ModelViewerPlane();
    void initialize(
        MeshManager& meshManager,
        IModelViewerCallback* callback
    );
    void update();
    void drawGUI();

private:
    ModelViewerPlane(const ModelViewerPlane&) = delete;
    ModelViewerPlane& operator=(const ModelViewerPlane&) = delete;

    void onChangeModel(const GameObject& newModel);

private:
    std::shared_ptr<GameObject> mPlane;
    bool mIsDraw;
};
