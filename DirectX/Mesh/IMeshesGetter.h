#pragma once

#include <list>
#include <memory>

class MeshRenderer;

using MeshPtr = std::shared_ptr<MeshRenderer>;
using MeshPtrList = std::list<MeshPtr>;

class IMeshesGetter {
public:
    virtual ~IMeshesGetter() = default;
    virtual const MeshPtrList& getMeshes() const = 0;
};
