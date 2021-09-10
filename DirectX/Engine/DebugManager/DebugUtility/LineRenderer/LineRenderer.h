#pragma once

#include "../../../../Math/Math.h"
#include <memory>
#include <string>

class VertexBuffer;
class IndexBuffer;

enum class DimensionType {
    TWO,
    THREE
};

class LineRenderer {
public:
    LineRenderer(DimensionType type);
    virtual ~LineRenderer();
    //シェーダー名を取得する
    virtual std::string getShaderName() = 0;
    //溜まっているライン情報を削除する
    virtual void clear() = 0;
    //すべてのラインを描画していく
    virtual void drawLines(const Matrix4& proj) const = 0;
    //描画前処理
    virtual void drawingPreprocessing(const Matrix4& proj) {};

    void initialize();
    void draw(const Matrix4& proj);

    static void finalize();

private:
    LineRenderer(const LineRenderer&) = delete;
    LineRenderer& operator=(const LineRenderer&) = delete;

    //バーテックスバッファを作成する
    void createVertexBuffer();
    //インデックスバッファを作成する
    void createIndexBuffer();

protected:
    DimensionType mDimensionType;
    int mShaderID;

    inline static VertexBuffer* mVertexBuffer2D = nullptr;
    inline static VertexBuffer* mVertexBuffer3D = nullptr;
    inline static IndexBuffer* mIndexBuffer = nullptr;
};
