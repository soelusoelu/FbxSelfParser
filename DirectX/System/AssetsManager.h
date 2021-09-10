#pragma once

#include "AssetsDirectoryPath.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Mesh;
class Texture;
class Shader;

class AssetsManager {
private:
    //シングルトンだからprivate
    AssetsManager();
public:
    ~AssetsManager();
    //アセット管理クラスのインスタンスを返す
    static AssetsManager& instance();
    //終了処理
    void finalize();

    //テクスチャを読み込む
    void loadTexture(const std::string& filename, const std::string& directoryPath = AssetsDirectoryPath::TEXTURE_PATH);
    //ファイルパスからテクスチャIDを取得する
    int createTexture(const std::string& filename, const std::string& directoryPath = AssetsDirectoryPath::TEXTURE_PATH);
    //テクスチャを追加する
    int addTexture(const std::shared_ptr<Texture>& texture);
    //IDからテクスチャを取得する
    const Texture& getTextureFromID(int id) const;

    //ファイルパスからシェーダーを取得する
    int createShader(const std::string& filename, const std::string& directoryPath = AssetsDirectoryPath::SHADER_PATH);
    //IDからシェーダーを取得する
    const Shader& getShaderFormID(int id) const;

    //メッシュを読み込む
    void loadMesh(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::MODEL_PATH);
    //メッシュを読み込む
    void loadMeshFromFilePath(const std::string& filePath);
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMesh(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::MODEL_PATH);
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMeshFromFilePath(const std::string& filePath);

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

    //読み込み済みのテクスチャか
    bool loadedTexture(const std::string& filePath, int* outID = nullptr) const;
    //読み込み済みのシェーダーか
    bool loadedShader(const std::string& filePath, int* outID = nullptr) const;
    //読み込み済みのメッシュか
    bool loadedMesh(const std::string& filePath) const;

private:
    struct TextureParam {
        std::shared_ptr<Texture> texture;
        std::string filePath;
        int id;
    };
    struct ShaderParam {
        std::shared_ptr<Shader> shader;
        std::string filePath;
        int id;
    };

    static inline AssetsManager* mInstance = nullptr;

    std::vector<TextureParam> mTextures;
    std::vector<ShaderParam> mShaders;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;
};
