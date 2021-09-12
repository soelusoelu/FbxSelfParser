#pragma once

#include "../../Math/Math.h"
#include <fstream>
#include <string>
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
    const std::vector<Vector3>& getNormals() const;

private:
    //頂点を読み込む
    void parseVertices(std::ifstream& inFile, const std::string& upperLine);
    //インデックスを読み込む
    void parseIndices(std::ifstream& inFile, const std::string& upperLine);
    //法線を読み込む
    void parseNormals(std::ifstream& inFile);
    //法線の値を読み込む
    void parseNormalValues(std::ifstream& inFile, const std::string& upperLine);
    //配列の要素数を取得する
    int getElementCount(const std::string& line) const;
    //配列の要素を文字列で取得する
    std::string getElementsString(const std::string& line, bool replaceCommaToSpace = true) const;

private:
    //頂点配列
    std::vector<Vector3> mVertices;
    //面法線用頂点配列
    std::vector<Vector3> mSurfaceVertices;
    //インデックス配列
    std::vector<unsigned short> mIndices;
    //面法線用インデックス配列
    std::vector<unsigned short> mSurfaceIndices;
    //法線配列
    std::vector<Vector3> mSurfaceNormals;
    //面の数
    unsigned mNumSurface;
};
