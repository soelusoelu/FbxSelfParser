#pragma once

#include "../../Device/FileOperator.h"
#include "../../Device/Function.h"
#include <functional>
#include <memory>
#include <vector>

class OBBCollider;

class ColliderOptionGUI
    : public FileOperator
{
    using OBBColliderPtr = std::shared_ptr<OBBCollider>;

public:
    ColliderOptionGUI(std::vector<OBBColliderPtr>& colliders);
    ~ColliderOptionGUI();
    //Guiを描画する
    void drawGui();
    //Guiメニューを開く
    void open(const std::shared_ptr<OBBCollider>& target);
    //Guiメニューを開いているか
    bool isOpened() const;
    //Guiメニューが閉じているか
    bool isClosed() const;
    //OKボタンが押された際のコールバック
    void onOkButton(const std::function<void()>& f);

private:
    ColliderOptionGUI(const ColliderOptionGUI&) = delete;
    ColliderOptionGUI& operator=(const ColliderOptionGUI&) = delete;

    void division();

private:
    std::shared_ptr<OBBCollider> mTarget;
    std::vector<OBBColliderPtr>& mColliders;
    int mDivisionCount;
    int mAxisButton;
    bool mIsOpened;
    Function<void()> mOnOkButton;

    static inline constexpr int AXIS_COUNT = 3;
    static inline constexpr int MIN_DIVISION_COUNT = 1;
    static inline constexpr int MAX_DIVISION_COUNT = 10;
};
