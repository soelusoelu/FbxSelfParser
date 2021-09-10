#pragma once

#include <memory>
#include <string>

class FbxObjects;

class FbxParser {
public:
    FbxParser();
    ~FbxParser();
    FbxParser(const FbxParser&) = delete;
    FbxParser& operator=(const FbxParser&) = delete;

    void parse(const std::string& filePath);
    const FbxObjects& getObjects() const;

private:
    bool isSkip(const std::string& line) const;

private:
    std::unique_ptr<FbxObjects> mObjects;
};
