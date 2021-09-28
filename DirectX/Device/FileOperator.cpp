#include "FileOperator.h"

FileOperator::FileOperator(const std::string& className)
    : mClassName(className)
{
}

FileOperator::~FileOperator() = default;

void FileOperator::writeAndRead(JsonObject& inObj, FileMode mode) {
    if (mode == FileMode::SAVE) {
        ////保存用オブジェクト作成
        //rapidjson::Value obj(rapidjson::kObjectType);
        ////保存用で関数呼び出し
        //saveAndLoad(obj, alloc, mode);
        ////保存用オブジェクトを追加
        //inObj.AddMember(rapidjson::StringRef(mClassName.c_str()), obj, alloc);
    } else if (mode == FileMode::LOAD) {
        const char* name = mClassName.c_str();
        //名前が登録されてるのを確認してから読み込み開始
        if (inObj.hasObject(name)) {
            auto& obj = inObj.getObject(name);
            saveAndLoad(obj, mode);
        }
    }

    childSaveAndLoad(inObj, mode);
}
