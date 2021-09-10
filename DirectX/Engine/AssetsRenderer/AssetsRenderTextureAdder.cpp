#include "AssetsRenderTextureAdder.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Device/Button.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/JsonHelper.h"

AssetsRenderTextureAdder::AssetsRenderTextureAdder()
    : FileOperator("AssetsRenderTextureAdder")
    , mButton(nullptr)
    , mAssetsAdder(nullptr)
    , mRenderPosition()
{
}

AssetsRenderTextureAdder::~AssetsRenderTextureAdder() = default;

void AssetsRenderTextureAdder::initialize(IAddAssets* adder) {
    mAssetsAdder = adder;
    mButton = std::make_unique<SpriteButton>([&] { onClickButton(); }, mSpriteFilePath, mRenderPosition);
}

void AssetsRenderTextureAdder::update() {
    mButton->clickButton(Input::mouse().getMousePosition());
}

void AssetsRenderTextureAdder::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mRenderPosition, "renderPosition", inObj, alloc, mode);
    JsonHelper::getSet(mSpriteFilePath, "spriteButtonFilePath", inObj, alloc, mode);
}

void AssetsRenderTextureAdder::onClickButton() {
    if (std::string outFilePath, outFileName; FileUtil::openFileDialog(outFilePath, outFileName)) {
        //絶対パスからアセットディレクトリ部分を抜き出す
        auto assetsDir = FileUtil::getAssetsFromAbsolutePath(outFilePath);

        std::string JSON = ".json";
        if (FileUtil::getFileExtension(assetsDir) == JSON) {
            auto filePath = assetsDir.substr(0, assetsDir.length() - JSON.length());
            auto filename = FileUtil::getFileNameFromDirectry(filePath);
            auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);
            auto gameObj = GameObjectCreater::create(filename, directoryPath);
            auto mesh = gameObj->componentManager().getComponent<MeshComponent>();
            if (!mesh) {
                return;
            }

            assetsDir = mesh->getFilePath();
        }

        mAssetsAdder->add(assetsDir);
    }
}
