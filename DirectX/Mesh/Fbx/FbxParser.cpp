#include "FbxParser.h"
#include "FbxObjects.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"
#include <fstream>
#include <sstream>
#include "FbxObject.h"
#include "FbxReader.h"
#include "FbxStream.h"

FbxParser::FbxParser()
    : mObjects(std::make_unique<FbxObjects>())
{
}

FbxParser::~FbxParser() = default;

void FbxParser::parse(const std::string& filePath) {
    FbxStream stream(filePath.c_str());
    FbxObject root;
    FbxReader reader;
    reader.parse(stream, root);
}

const FbxObjects& FbxParser::getObjects() const {
    return *mObjects;
}

bool FbxParser::isSkip(const std::string& line) const {
    //文字列が空ならスキップ
    if (line.empty()) {
        return true;
    }
    //文字列の先頭がセミコロン(コメント)ならスキップ
    if (line[0] == ';') {
        return true;
    }

    //スキップしない
    return false;
}
