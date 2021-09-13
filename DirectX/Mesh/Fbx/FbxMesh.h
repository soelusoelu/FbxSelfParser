﻿#pragma once

#include "../../Math/Math.h"
#include <fstream>
#include <string>
#include <vector>

class FbxMesh {
public:
    FbxMesh();
    ~FbxMesh();
    FbxMesh(const FbxMesh&) = delete;
    FbxMesh& operator=(const FbxMesh&) = delete;

    void parse(std::ifstream& inFile);
    const std::vector<Vector3>& getVertices() const;
    const std::vector<unsigned short>& getIndices() const;
    const std::vector<Vector3>& getNormals() const;
    const std::vector<Vector2>& getUVs() const;
    unsigned getNumSurface() const;

private:
    //頂点を読み込む
    void parseVertices(std::ifstream& inFile, const std::string& upperLine);
    //インデックスを読み込む
    void parseIndices(std::ifstream& inFile, const std::string& upperLine);
    //法線を読み込む
    void parseNormals(std::ifstream& inFile);
    //法線の値を読み込む
    void parseNormalValues(std::ifstream& inFile, const std::string& upperLine);
    //UVを読み込む
    void parseUV(std::ifstream& inFile);
    //UVの値を読み込む
    void parseUVValues(std::ifstream& inFile, const std::string& upperLine);
    //UVインデックスを読み込む
    void parseUVIndices(std::ifstream& inFile, const std::string& upperLine);
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
    //UV配列
    std::vector<Vector2> mUVs;
    //面法線用UV配列
    std::vector<Vector2> mSurfaceUVs;
    //UVインデックス配列
    std::vector<unsigned short> mUVIndices;
    //面の数
    unsigned mNumSurface;
};
