#pragma once

class ShaderBinder {
private:
    //静的クラスのため
    ShaderBinder() = delete;
    ~ShaderBinder() = delete;

public:
    //シェーダーをバインドする
    static void bind(int id);

private:
    ShaderBinder(const ShaderBinder&) = delete;
    ShaderBinder& operator=(const ShaderBinder&) = delete;

private:
    //前回描画時のシェーダーID
    static inline int mPreviousShaderID = -1;
};
