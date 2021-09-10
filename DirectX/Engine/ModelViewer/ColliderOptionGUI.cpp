#include "ColliderOptionGUI.h"
#include "../DebugManager/Inspector/ImGuiWrapper.h"
#include "../../Component/Engine/Collider/OBBCollider.h"
#include "../../Input/Input.h"
#include <algorithm>
#include <string>

ColliderOptionGUI::ColliderOptionGUI(std::vector<OBBColliderPtr>& colliders)
    : FileOperator("ColliderOptionGUI")
    , mTarget(nullptr)
    , mColliders(colliders)
    , mDivisionCount(MIN_DIVISION_COUNT)
    , mAxisButton(0)
    , mIsOpened(false)
{
}

ColliderOptionGUI::~ColliderOptionGUI() = default;

void ColliderOptionGUI::drawGui() {
    if (isClosed()) {
        return;
    }

    //マウス位置を補正する
    const auto& mousePos = Input::mouse().getMousePosition();
    ImGui::GetIO().MousePos = ImVec2(mousePos.x, mousePos.y);
    //ウィンドウ位置
    ImGui::SetNextWindowPos(ImVec2(800.f, 256.f), ImGuiCond_Once);
    //ウィンドウサイズ
    ImGui::SetNextWindowSize(ImVec2(256.f, 512.f), ImGuiCond_Once);
    //ウィンドウを不透明に
    ImGui::SetNextWindowBgAlpha(1.f);

    ImGui::Begin("OBBOption");

    ImGuiWrapper::sliderInt("division count", mDivisionCount, MIN_DIVISION_COUNT, MAX_DIVISION_COUNT);
    ImGui::Text("dividing axis");
    std::string axisLabels[AXIS_COUNT] = { "X", "Y", "Z" };
    ImGuiWrapper::radioButton(axisLabels, mAxisButton, AXIS_COUNT);

    if (ImGui::Button("Cancel")) {
        mIsOpened = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("OK")) {
        division();
        mIsOpened = false;

        mOnOkButton();
    }

    ImGui::End();
}

void ColliderOptionGUI::open(const std::shared_ptr<OBBCollider>& target) {
    mTarget = target;
    mIsOpened = true;
}

bool ColliderOptionGUI::isOpened() const {
    return mIsOpened;
}

bool ColliderOptionGUI::isClosed() const {
    return !mIsOpened;
}

void ColliderOptionGUI::onOkButton(const std::function<void()>& f) {
    mOnOkButton += f;
}

void ColliderOptionGUI::division() {
    //対象コライダーを分割して追加する
    const auto boneNo = mTarget->getBone().number;
    for (int i = 0; i < mDivisionCount; ++i) {
        auto obb = mTarget->addComponent<OBBCollider>("OBBCollider");
        float start = static_cast<float>(i) / static_cast<float>(mDivisionCount);
        float end = static_cast<float>(i + 1) / static_cast<float>(mDivisionCount);
        obb->setBone(boneNo, start, end);
        mColliders.emplace_back(obb);
    }

    //配列からコライダーを探して削除
    auto itr = std::find(mColliders.begin(), mColliders.end(), mTarget);
    if (itr != mColliders.end()) {
        std::iter_swap(itr, mColliders.end() - 1);
        mColliders.pop_back();
    }

    //コライダーに削除命令
    mTarget->destroy();
    mTarget = nullptr;
}
