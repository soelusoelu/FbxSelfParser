#pragma once

#include "IModelViewerCallback.h"
#include <memory>

class GameObject;
class SkinMeshComponent;

class AnimationViewer {
public:
    AnimationViewer();
    ~AnimationViewer();
    //初期化
    void initialize(IModelViewerCallback* callback);
    //毎フレーム更新
    void update();
    //姿勢をTポーズにする
    void setTPose();

private:
    AnimationViewer(const AnimationViewer&) = delete;
    AnimationViewer& operator=(const AnimationViewer&) = delete;

    //モーションを変更するか
    bool isChangeMotion(int& nextMotionNumber) const;
    //モデル変更時
    void onChangeModel(const GameObject& newModel);

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
};
