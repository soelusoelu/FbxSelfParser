#pragma once

#include "Reader/FbxObject.h"
#include <memory>
#include <string>

class FbxReader;
class FbxMesh;

class FbxParser {
public:
    FbxParser();
    ~FbxParser();
    FbxParser(const FbxParser&) = delete;
    FbxParser& operator=(const FbxParser&) = delete;

    void parse(const std::string& filePath);

    const FbxObject& getRootObject() const;
    const FbxObject& getObject(const std::string& name) const;
    const FbxMesh& getMesh() const;

private:
    std::unique_ptr<FbxReader> mReader;
    std::unique_ptr<FbxObject> mRootObject;
    std::unique_ptr<FbxMesh> mMeshObject;
};
