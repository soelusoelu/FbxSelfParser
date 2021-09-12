#include "FbxMesh.h"
#include <algorithm>
#include <sstream>

FbxMesh::FbxMesh()
    : mNumSurface(0)
{
}

FbxMesh::~FbxMesh() = default;

void FbxMesh::parse(std::ifstream& inFile) {
    std::string line;
    while (std::getline(inFile, line)) {
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
            mNumSurface = mIndices.size() / 3;
        } else if (key == "LayerElementNormal") {
            parseNormals(inFile);
            break;
        }
    }

    unsigned num = mNumSurface * 3;
    mSurfaceVertices.resize(num);
    mSurfaceIndices.resize(num);

    for (size_t i = 0; i < mNumSurface; ++i) {
        auto idx = i * 3;
        mSurfaceIndices[idx] = idx;
        mSurfaceIndices[idx + 1] = idx + 1;
        mSurfaceIndices[idx + 2] = idx + 2;
    }
    for (size_t i = 0; i < num; ++i) {
        mSurfaceVertices[i] = mVertices[mIndices[i]];
    }
}

const std::vector<Vector3>& FbxMesh::getVertices() const {
    return mSurfaceVertices;
}

const std::vector<unsigned short>& FbxMesh::getIndices() const {
    return mSurfaceIndices;
}

const std::vector<Vector3>& FbxMesh::getNormals() const {
    return mSurfaceNormals;
}

unsigned FbxMesh::getNumSurface() const {
    return mNumSurface;
}

void FbxMesh::parseVertices(std::ifstream& inFile, const std::string& upperLine) {
    //文字列から数値を取り出し頂点数を取得する(3はVector3の要素数)
    int numVertices = getElementCount(upperLine) / 3;
    mVertices.resize(numVertices);

    //頂点配列の添字(複数回ファイルにアクセスするためループの外で確保)
    int idx = 0;
    //ファイルから読み込んだ1行がここに入る
    std::string line;
    while (std::getline(inFile, line)) {
        //文字列の最後がスペースかカンマなら削除
        while (line.back() == ' ' || line.back() == ',') {
            line.erase(line.end() - 1);
        }
        //文字列の最後が } ならループを抜ける
        if (line.back() == '}') {
            break;
        }

        auto elementsStr = getElementsString(line);
        std::istringstream lineStream(elementsStr);

        while (idx < numVertices) {
            //本読み込み
            auto& v = mVertices[idx];
            lineStream >> v.x >> v.y >> v.z;

            ++idx;

            if (lineStream.eof()) {
                break;
            }
        }
    }
}

void FbxMesh::parseIndices(std::ifstream& inFile, const std::string& upperLine) {
    //文字列から数値を取り出しインデックス数を取得する
    int numIndices = getElementCount(upperLine);
    mIndices.resize(numIndices);

    //頂点配列の添字(複数回ファイルにアクセスするためループの外で確保)
    int idx = 0;
    //ファイルから読み込んだ1行がここに入る
    std::string line;
    while (std::getline(inFile, line)) {
        //文字列の最後がスペースかカンマなら削除
        while (line.back() == ' ' || line.back() == ',') {
            line.erase(line.end() - 1);
        }
        //文字列の最後が } ならループを抜ける
        if (line.back() == '}') {
            break;
        }

        auto elementsStr = getElementsString(line);
        std::istringstream lineStream(elementsStr);

        while (idx < numIndices) {
            //本読み込み
            int tmp = 0;
            lineStream >> tmp;

            //何故かマイナスのインデックスがあるから調整する
            if (tmp < 0) {
                tmp = Math::abs(tmp) - 1;
            }

            mIndices[idx] = static_cast<unsigned short>(tmp);

            ++idx;

            if (lineStream.eof()) {
                break;
            }
        }
    }
}

void FbxMesh::parseNormals(std::ifstream& inFile) {
    std::string line;
    while (std::getline(inFile, line)) {
        auto foundPos = line.find(':');
        auto key = line.substr(0, foundPos);

        //タブを削除する
        while (key[0] == '\t') {
            key.erase(key.begin());
        }

        if (key == "Normals") {
            parseNormalValues(inFile, line);
            return;
        }
    }
}

void FbxMesh::parseNormalValues(std::ifstream& inFile, const std::string& upperLine) {
    //文字列から数値を取り出し頂点数を取得する(9はVector3の要素数)
    int numNormals = getElementCount(upperLine) / 3;
    mSurfaceNormals.resize(numNormals);

    //法線配列の添字(複数回ファイルにアクセスするためループの外で確保)
    int idx = 0;
    //ファイルから読み込んだ1行がここに入る
    std::string line;
    while (std::getline(inFile, line)) {
        //文字列の最後がスペースかカンマなら削除
        while (line.back() == ' ' || line.back() == ',') {
            line.erase(line.end() - 1);
        }
        //文字列の最後が } ならループを抜ける
        if (line.back() == '}') {
            break;
        }

        auto elementsStr = getElementsString(line);
        std::istringstream lineStream(elementsStr);

        while (idx < numNormals) {
            //本読み込み
            auto& n = mSurfaceNormals[idx];
            lineStream >> n.x >> n.y >> n.z;

            ++idx;

            if (lineStream.eof()) {
                break;
            }
        }
    }
}

int FbxMesh::getElementCount(const std::string& line) const {
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

std::string FbxMesh::getElementsString(const std::string& line, bool replaceCommaToSpace) const {
    std::string result(line);

    //a:の次から始まる文字列を取得する(+2はa:の文字数分)
    auto foundPos = line.find("a:");
    if (foundPos != std::string::npos) {
        result = line.substr(foundPos + 2);
    }
    //スペースを削除する
    while (result[0] == ' ') {
        result.erase(result.begin());
    }

    if (replaceCommaToSpace) {
        //カンマをスペースに書き換える
        std::replace(result.begin(), result.end(), ',', ' ');
    }

    return result;
}
