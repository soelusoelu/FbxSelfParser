#pragma once

#include <memory>

class MeshRenderer;

using MeshPtr = std::shared_ptr<MeshRenderer>;

class IMeshAdder {
public:
    virtual ~IMeshAdder() = default;
    virtual void add(const MeshPtr& mesh, bool handleShadow) = 0;
};
