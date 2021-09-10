#include "FbxObjects.h"
#include <algorithm>
#include <sstream>

FbxObjects::FbxObjects() = default;

FbxObjects::~FbxObjects() = default;

void FbxObjects::parse(std::ifstream& inFile) {
    while (!inFile.eof()) {
        //1行ずつ読み込む
        std::string line;
        std::getline(inFile, line);

        auto foundPos = line.find(':');
        auto key = line.substr(0, foundPos);

        //タブを削除する
        while (key[0] == '\t') {
            key.erase(key.begin());
        }

        if (key == "Vertices") {
            parseVertices(inFile, line);
        } else if (key == "PolygonVertexIndex") {
            parseIndices(inFile, line);
        }
    }
}

const std::vector<Vector3>& FbxObjects::getVertices() const {
    return mVertices;
}

const std::vector<unsigned short>& FbxObjects::getIndices() const {
    return mIndices;
}

void FbxObjects::parseVertices(std::ifstream& inFile, const std::string& upperLine) {
    //文字列から数値を取り出し頂点数を取得する
    int numVertices = getElementCount(upperLine) / 3;
    mVertices.resize(numVertices);

    //1行読み込む
    std::string line;
    std::getline(inFile, line);

    //a:の次から始まる文字列を取得する
    auto foundPos = line.find("a:") + 2;
    auto substr = line.substr(foundPos);
    //スペースを削除する
    while (substr[0] == ' ') {
        substr.erase(substr.begin());
    }

    //カンマをスペースに書き換える
    std::replace(substr.begin(), substr.end(), ',', ' ');

    //1行分の文字列をストリームに変換して解析しやすくする
    std::istringstream lineStream(substr);

    for (auto&& v : mVertices) {
        lineStream >> v.x >> v.y >> v.z;
    }
}

void FbxObjects::parseIndices(std::ifstream& inFile, const std::string& upperLine) {
    //文字列から数値を取り出しインデックス数を取得する
    int numIndices = getElementCount(upperLine);
    mIndices.resize(numIndices);

    //1行読み込む
    std::string line;
    std::getline(inFile, line);

    //a:の次から始まる文字列を取得する
    auto foundPos = line.find("a:") + 2;
    auto substr = line.substr(foundPos);
    //スペースを削除する
    while (substr[0] == ' ') {
        substr.erase(substr.begin());
    }

    //カンマをスペースに書き換える
    std::replace(substr.begin(), substr.end(), ',', ' ');

    //1行分の文字列をストリームに変換して解析しやすくする
    std::istringstream lineStream(substr);

    for (auto&& i : mIndices) {
        int tmp = 0;
        lineStream >> tmp;

        //何故かマイナスのインデックスがあるから調整する
        if (tmp < 0) {
            tmp = Math::abs(tmp) - 1;
        }

        i = static_cast<unsigned short>(tmp);
    }
}

int FbxObjects::getElementCount(const std::string& line) const {
    //*の次から始まる文字列を取得する
    auto foundPos = line.rfind('*') + 1;
    auto substr = line.substr(foundPos);

    //切り取った文字列からスペースまでの文字列を取得する
    //これで数値部分の文字列が取得できる
    foundPos = substr.find(' ');
    substr = substr.substr(0, foundPos);

    //文字列を数値に変換する
    int num = std::stoi(substr);

    return num;
}
