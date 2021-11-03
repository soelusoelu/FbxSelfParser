#pragma once

#include "AssetsDirectoryPath.h"
#include "../Mesh/Material.h"
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
    //IDからテクスチャのファイルパスを取得する
    const std::string& getTextureFilePathFromID(int id) const;

    //ファイルパスからシェーダーIDを取得する
    int createShader(const std::string& filename, const std::string& directoryPath = AssetsDirectoryPath::SHADER_PATH);
    //IDからシェーダーを取得する
    const Shader& getShaderFromID(int id) const;

    //マテリアルからマテリアルIDを取得する
    int createMaterial(const Material& material);
    //マテリアルを変更する
    void changeMaterial(int id, const Material& material);
    //IDからマテリアルを取得する
    const Material& getMaterialFromID(int id) const;
    //マテリアル名からマテリアルを取得する
    const Material* getMaterialFromName(const std::string& name) const;
    //マテリアル名からマテリアルIDを取得する
    int getMaterialIDFromName(const std::string& name) const;

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
    //読み込み済みのマテリアルか
    bool loadedMaterial(const std::string& name, int* outID = nullptr) const;
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
    std::vector<Material> mMaterials;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;
};
