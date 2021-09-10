#pragma once

#include "IMeshLoader.h"
#include "Material.h"
#include "../Math/Math.h"
#include <vector>

struct Triangle;

//外部公開用メッシュインターフェース
class IMesh {
public:
    virtual ~IMesh() = default;
    //指定のマテリアルを設定する
    virtual void setMaterial(const Material& material, unsigned index) = 0;
    //指定のマテリアルの取得する
    virtual const Material& getMaterial(unsigned index) const = 0;
    //メッシュの数を取得する
    virtual unsigned getMeshCount() const = 0;
    //指定の頂点情報を取得する
    virtual const MeshVertices& getMeshVertices(unsigned index) const = 0;
    //指定の頂点位置配列を取得する
    virtual const MeshVerticesPosition& getMeshVerticesPosition(unsigned index) const = 0;
    //指定のインデックスバッファを取得する
    virtual const Indices& getMeshIndices(unsigned index) const = 0;
    //指定のメッシュのポリゴン数を取得する
    virtual unsigned getPolygonCount(unsigned index) const = 0;
    //指定のメッシュの指定のポリゴンを取得する
    virtual Triangle getPolygon(unsigned meshIndex, unsigned polygonIndex) const = 0;
    //指定のメッシュの指定のポリゴンにワールド行列を演算し取得する
    virtual Triangle getPolygon(unsigned meshIndex, unsigned polygonIndex, const Matrix4& world) const = 0;
    //指定のメッシュに頂点情報を設定する
    virtual void setMeshVertices(const MeshVertices& newMeshVertices, unsigned index) = 0;
    //メッシュを描画するかを指定する
    virtual void setMeshActive(unsigned index, bool value) = 0;
    //メッシュを描画するかを取得する
    virtual bool getMeshActive(unsigned index) const = 0;
};
