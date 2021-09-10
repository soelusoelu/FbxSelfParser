#pragma once

#include "Scene.h"

class MapEditor : public Scene {
public:
    MapEditor();
    ~MapEditor();
    virtual void awake() override;
    virtual void update() override;

private:
    MapEditor(const MapEditor&) = delete;
    MapEditor& operator=(const MapEditor&) = delete;
};
