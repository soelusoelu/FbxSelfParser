#pragma once

#include <string>

class ITextureReader {
public:
    virtual ~ITextureReader() = default;
    virtual void read(const std::string& filePath) = 0;
};
