#include "FileOperator.h"
#include "../System/Json/JsonValue.h"
#include <cassert>

FileOperator::FileOperator(const std::string& className)
    : mClassName(className)
{
}

FileOperator::~FileOperator() = default;

void FileOperator::writeAndRead(JsonObject& inObj, FileMode mode) {
    if (mode == FileMode::SAVE) {
        //名前が登録されているなら上書き
        if (inObj.hasValue(mClassName)) {
            auto& value = inObj.getValue(mClassName);
            assert(value.isObject());
            saveAndLoad(value.getObject(), mode);
        }
        //名前が登録されていないなら新規書き込み
        else {
            //保存用オブジェクト作成
            auto value = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
            //保存用で関数呼び出し
            saveAndLoad(value->getObject(), mode);
            //保存用オブジェクトを追加
            inObj.setValue(mClassName, value);
        }
    } else if (mode == FileMode::LOAD) {
        //名前が登録されてるのを確認してから読み込み開始
        if (inObj.hasValue(mClassName)) {
            auto& value = inObj.getValue(mClassName);
            assert(value.isObject());
            saveAndLoad(value.getObject(), mode);
        }
    }

    childSaveAndLoad(inObj, mode);
}
