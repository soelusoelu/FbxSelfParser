#pragma once

#include "../../Math/Math.h"
#include <fstream>
#include <vector>

class FbxObjects {
public:
    FbxObjects();
    ~FbxObjects();
    FbxObjects(const FbxObjects&) = delete;
    FbxObjects& operator=(const FbxObjects&) = delete;

    void parse(std::ifstream& inFile);
    const std::vector<Vector3>& getVertices() const;
    const std::vector<unsigned short>& getIndices() const;

private:
    void parseVertices(std::ifstream& inFile, const std::string& upperLine);
    void parseIndices(std::ifstream& inFile, const std::string& upperLine);
    int getElementCount(const std::string& line) const;

private:
    //頂点配列
    std::vector<Vector3> mVertices;
    //インデックス配列
    std::vector<unsigned short> mIndices;
};
