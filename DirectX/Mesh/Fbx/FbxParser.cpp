#include "FbxParser.h"
#include "FbxObjects.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"
#include <fstream>
#include <sstream>

FbxParser::FbxParser()
    : mObjects(std::make_unique<FbxObjects>())
{
}

FbxParser::~FbxParser() = default;

void FbxParser::parse(const std::string& filePath) {
    //fbxファイルを開く
    std::ifstream ifs(filePath, std::ios_base::in | std::ios_base::binary);
    if (ifs.fail()) {
        Debug::windowMessage(filePath + " : ファイルが存在しません");
        return;
    }

    //解析開始
    while (!ifs.eof()) {
        //1行ずつ読み込む
        std::string line;
        std::getline(ifs, line);

        //解析に必要な行か判定
        if (isSkip(line)) {
            continue;
        }

        //1行分の文字列をストリームに変換して解析しやすくする
        std::istringstream lineStream(line);

        //半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(lineStream, key, ':');

        if (key == "Objects") {
            std::getline(ifs, line);
            if (line.rfind("{") != std::string::npos) {
                mObjects->parse(ifs);
            }
        }
    }
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
