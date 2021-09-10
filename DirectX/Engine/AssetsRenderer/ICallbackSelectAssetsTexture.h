#pragma once

#include <functional>

class ICallbackSelectAssetsTexture {
public:
    virtual ~ICallbackSelectAssetsTexture() = default;
    //セレクトを選択した際のコールバック
    virtual void callbackSelectTexture(const std::function<void()>& callback) = 0;
};
