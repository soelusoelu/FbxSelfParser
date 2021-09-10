#pragma once

#include "../../../Device/FileOperator.h"
#include "../../../Math/Math.h"
#include <memory>

class DrawString;
class Renderer;
class Log;
class PointRenderer;
class LineRenderer2D;
class LineInstancingDrawer;

class DebugUtility
    : public FileOperator
{
private:
    DebugUtility();

public:
    ~DebugUtility();
    static DebugUtility& instance();
    void initialize();
    void finalize();
    void preUpdateProcess();
    void draw(DrawString& drawer) const;
    void drawLine2D(const Renderer& renderer, Matrix4& proj) const;
    void draw3D(const Renderer& renderer, const Matrix4& viewProj) const;
    Log& log() const;
    PointRenderer& pointRenderer() const;
    LineRenderer2D& lineRenderer2D() const;
    LineInstancingDrawer& lineRenderer3D() const;

private:
    DebugUtility(const DebugUtility&) = delete;
    DebugUtility& operator=(const DebugUtility&) = delete;

    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    static inline DebugUtility* mInstance = nullptr;

    std::unique_ptr<Log> mLog;
    std::unique_ptr<PointRenderer> mPointRenderer;
    std::unique_ptr<LineRenderer2D> mLineRenderer2D;
    std::unique_ptr<LineInstancingDrawer> mLineRenderer3D;
};
