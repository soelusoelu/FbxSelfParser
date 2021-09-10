#pragma once

#include <string>

class IAddAssets {
public:
    ~IAddAssets() = default;
    virtual void add(const std::string& filePath) = 0;
    virtual void add(const std::string& fileName, const std::string& directoryPath) = 0;
};