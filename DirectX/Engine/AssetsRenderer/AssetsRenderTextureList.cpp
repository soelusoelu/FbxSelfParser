#include "AssetsRenderTextureList.h"
#include "AssetsRenderTexture.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../System/Window.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/JsonHelper.h"
#include <vector>

AssetsRenderTextureList::AssetsRenderTextureList()
    : FileOperator("AssetsRenderTextureList")
    , mTextureSize(0)
    , mColumnDisplayLimit(0)
    , mTextureDisplayInterval(0)
{
}

AssetsRenderTextureList::~AssetsRenderTextureList() = default;

void AssetsRenderTextureList::add(const std::string& filePath) {
    //パスが読み込み済みなら終了
    if (loadedFilePath(filePath)) {
        return;
    }

    //テクスチャを追加
    createTexture(filePath);
    //パスを追加
    mTexturesFilePath.emplace(filePath);
}

void AssetsRenderTextureList::add(const std::string& fileName, const std::string& directoryPath) {
    add(directoryPath + fileName);
}

void AssetsRenderTextureList::deleteTexture(const std::string& filePath) {
    auto itr = mTextures.begin();
    while (itr != mTextures.end()) {
        if (filePath == (*itr)->getTexture().getFilePath()) {
            itr = mTextures.erase(itr);
            mTexturesFilePath.erase(filePath);
            break;
        } else {
            ++itr;
        }
    }

    //テクスチャを並び替える
    rearrangeTextures();
}

const AssetsRenderTexturePtrArray& AssetsRenderTextureList::getTextures() const {
    return mTextures;
}

void AssetsRenderTextureList::initialize() {
    //ファイルから読み込んだパスでテクスチャを作成する
    for (const auto& path : mTexturesFilePath) {
        createTexture(path);
    }
    //メッシュをテクスチャに描画する
    //drawMeshOnTexture();
}

void AssetsRenderTextureList::drawMeshOnTexture() {
    //まだ描画していないメッシュをテクスチャに描画する
    for (const auto& tex : mNonDrawTextures) {
        tex->drawMesh();

        mTextures.emplace_back(tex);
    }

    mNonDrawTextures.clear();
}

void AssetsRenderTextureList::drawTexture(const Matrix4& proj) const {
    for (const auto& tex : mTextures) {
        tex->getTexture().draw(proj);
    }
}

void AssetsRenderTextureList::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mTextureSize, "textureSize", inObj, alloc, mode);
    JsonHelper::getSet(mTextureDisplayInterval, "textureDisplayInterval", inObj, alloc, mode);

    if (mode == FileMode::SAVE) {
        std::vector<std::string> temp(mTexturesFilePath.cbegin(), mTexturesFilePath.cend());
        JsonHelper::setStringArray(temp, "texturesFilePath", inObj, alloc);
    } else {
        mColumnDisplayLimit = Window::width() / (mTextureSize + mTextureDisplayInterval);

        std::vector<std::string> temp;
        if (JsonHelper::getStringArray(temp, "texturesFilePath", inObj)) {
            mTexturesFilePath.insert(temp.cbegin(), temp.cend());
        }
    }
}

void AssetsRenderTextureList::createTexture(const std::string& fileName, const std::string& directoryPath) {
    createTexture(directoryPath + fileName);
}

void AssetsRenderTextureList::createTexture(const std::string& filePath) {
    const auto& newTex = std::make_shared<AssetsRenderTexture>(filePath, mTextureSize);

    //テクスチャの位置調整
    setTexturePosition(*newTex, mTextures.size() + mNonDrawTextures.size());

    //テクスチャを追加
    mNonDrawTextures.emplace_back(newTex);
}

void AssetsRenderTextureList::rearrangeTextures() {
    for (size_t i = 0; i < mTextures.size(); ++i) {
        setTexturePosition(*mTextures[i], i);
    }
}

void AssetsRenderTextureList::setTexturePosition(AssetsRenderTexture& target, int textureNo) {
    target.getTexture().setPositionForTexture(Vector2(
        (mTextureSize + mTextureDisplayInterval) * (textureNo % mColumnDisplayLimit) + mTextureDisplayInterval,
        (mTextureSize + mTextureDisplayInterval) * (textureNo / mColumnDisplayLimit) + Window::height() + mTextureDisplayInterval
    ));
}

bool AssetsRenderTextureList::loadedFilePath(const std::string& filePath) const {
    return (mTexturesFilePath.find(filePath) != mTexturesFilePath.end());
}
