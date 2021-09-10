#include "SaveModelViewerCollider.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/LevelLoader.h"
#include <string>

void SaveModelViewerCollider::save(const GameObject& target) {
    //保存するディレクトリとファイル名を取得する
    std::string filePath, filename;
    if (FileUtil::saveFileDialog(filePath, filename)) {
        //拡張子が指定されていなければ付け足す
        if (filename.rfind('.') == std::string::npos) {
            filename += ".json";
        }

        //ファイルパスから必要な部分を抜き出す
        auto savePath = FileUtil::getAssetsFromAbsolutePath(filePath);
        auto saveDirectory = FileUtil::getDirectryFromFilePath(savePath);

        //保存
        LevelLoader::saveOnlyComponents(target, filename, saveDirectory);
    }
}
