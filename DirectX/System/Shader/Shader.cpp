#include "Shader.h"
#include "ConstantBufferManager.h"
#include "InputElementManager.h"
#include "../GlobalFunction.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/StringUtil.h"
#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

Shader::Shader(const std::string& fileName, const std::string& directoryPath) :
    mShaderName(fileName),
    mVSBlob(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mVertexLayout(nullptr) {
    if (!constantBufferManager || !inputElementManager) {
        constantBufferManager = new ConstantBufferManager();
        inputElementManager = new InputElementManager();
    }

    //ディレクトパスとファイル名を結合する
    auto filePath = directoryPath + fileName;

    createVertexShader(filePath);
    createPixelShader(filePath);
    mConstantBuffers = constantBufferManager->createConstantBuffer(fileName);
    createInputLayout(inputElementManager->createInputLayout(fileName));
}

Shader::~Shader() = default;

void Shader::finalize() {
    safeDelete(constantBufferManager);
    safeDelete(inputElementManager);
}

ID3D11VertexShader* Shader::getVertexShader() const {
    return mVertexShader.Get();
}

ID3D11PixelShader* Shader::getPixelShader() const {
    return mPixelShader.Get();
}

const InputElement& Shader::getVertexLayout() const {
    return *mVertexLayout;
}

const Buffer& Shader::getConstantBuffer(unsigned index) const {
    assert(index < mConstantBuffers.size());
    return *mConstantBuffers[index];
}

const std::string& Shader::getShaderName() const {
    return mShaderName;
}

void Shader::createVertexShader(const std::string& fileName) {
    //シェーダーファイルをコンパイルする
    if (!compileShader(&mVSBlob, fileName, "VS", "vs_5_0")) {
        return;
    }

    //ピクセルシェーダー作成
    auto res = MyDirectX::DirectX::instance().device()->CreateVertexShader(
        mVSBlob->GetBufferPointer(),
        mVSBlob->GetBufferSize(),
        nullptr,
        &mVertexShader
    );

    //失敗していたらエラーメッセージ発行
    if (FAILED(res)) {
        Debug::windowMessage(fileName + ": バーテックスシェーダー作成失敗");
    }
}

void Shader::createPixelShader(const std::string& fileName) {
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

    //シェーダーファイルをコンパイルする
    if (!compileShader(&psBlob, fileName, "PS", "ps_5_0")) {
        return;
    }

    //ピクセルシェーダー作成
    auto res = MyDirectX::DirectX::instance().device()->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &mPixelShader
    );

    //失敗していたらエラーメッセージ発行
    if (FAILED(res)) {
        Debug::windowMessage(fileName + ": ピクセルシェーダー作成失敗");
    }
}

bool Shader::compileShader(Microsoft::WRL::ComPtr<ID3DBlob>* out, const std::string& fileName, const std::string& entryPoint, const std::string& target) {
    //Unicodeへ変換する
    auto wcharFileName = StringUtil::charToWchar(fileName);

    //コンパイル開始
    auto hresult = D3DCompileFromFile(
        wcharFileName.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint.c_str(),
        target.c_str(),
#ifdef _DEBUG
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
        0,
#endif // _DEBUG
        0,
        &*out,
        nullptr
    );

    //失敗していたらエラーメッセージ発行
    auto res = SUCCEEDED(hresult);
    if (!res) {
        Debug::windowMessage(fileName + ": hlsl読み込み失敗");
    }

    return res;
}

void Shader::createInputLayout(const std::vector<InputElementDesc>& layout) {
    mVertexLayout = std::make_unique<InputElement>(layout, mVSBlob.Get());
}
