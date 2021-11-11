#pragma once

#include <string>
#include <vector>

class ITextureReader {
public:
    virtual ~ITextureReader() = default;
    virtual void read(
        const std::string& filePath,
        std::vector<unsigned char>& colors
    ) = 0;

    virtual unsigned getWidth() const = 0;
    virtual unsigned getHeight() const = 0;
};
