#include "FbxObjects.h"
#include <algorithm>
#include <sstream>

FbxObjects::FbxObjects()
    : mNumSurface(0)
{
}

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
            mNumSurface = mIndices.size() / 3;
        } else if (key == "LayerElementNormal") {
            parseNormals(inFile);
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

const std::vector<Vector3>& FbxObjects::getVertices() const {
    return mSurfaceVertices;
}

const std::vector<unsigned short>& FbxObjects::getIndices() const {
    return mSurfaceIndices;
}

const std::vector<Vector3>& FbxObjects::getNormals() const {
    return mSurfaceNormals;
}

void FbxObjects::parseVertices(std::ifstream& inFile, const std::string& upperLine) {
    //文字列から数値を取り出し頂点数を取得する
    int numVertices = getElementCount(upperLine) / 3;
    mVertices.resize(numVertices);

    //1行読み込む
    std::string line;
    std::getline(inFile, line);

    //全要素を文字列で取得する
    auto elementsStr = getElementsString(line);

    //1行分の文字列をストリームに変換して解析しやすくする
    std::istringstream lineStream(elementsStr);

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

    //全要素を文字列で取得する
    auto elementsStr = getElementsString(line);

    //1行分の文字列をストリームに変換して解析しやすくする
    std::istringstream lineStream(elementsStr);

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

void FbxObjects::parseNormals(std::ifstream& inFile) {
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

void FbxObjects::parseNormalValues(std::ifstream& inFile, const std::string& upperLine) {
    //文字列から数値を取り出し頂点数を取得する(9はVector3の要素数 * ポリゴン頂点数)
    int numNormals = getElementCount(upperLine) / 9;
    mSurfaceNormals.resize(numNormals);

    //法線配列の添字(複数回ファイルにアクセスするためループの外で確保)
    int idx = 0;
    //読み込み途中か
    bool readIntermediate = false;
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

        //読み込み途中なら残りの分を読み飛ばす
        if (readIntermediate) {
            float tmp = 0.f;
            lineStream >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp;
        }

        while (idx < numNormals) {
            //本読み込み
            auto& n = mSurfaceNormals[idx];
            lineStream >> n.x >> n.y >> n.z;

            ++idx;

            if (lineStream.eof()) {
                readIntermediate = true;
                break;
            }

            //3頂点分法線が並んでいるが全部値が一緒なので読み飛ばす
            float tmp = 0.f;
            lineStream >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp;

            if (lineStream.eof()) {
                readIntermediate = false;
                break;
            }
        }
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

std::string FbxObjects::getElementsString(const std::string& line, bool replaceCommaToSpace) const {
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
