#pragma once

class TextureBinder {
private:
    //静的クラスのため
    TextureBinder() = delete;
    ~TextureBinder() = delete;

public:
    //テクスチャをシェーダーにバインドする
    static void bind(int id);

private:
    TextureBinder(const TextureBinder&) = delete;
    TextureBinder& operator=(const TextureBinder&) = delete;

private:
    //前回描画時のテクスチャID
    static inline int mPreviousTextureID = -1;
};
