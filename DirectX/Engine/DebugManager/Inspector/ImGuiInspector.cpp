#include "ImGuiInspector.h"
#include "../../../Component/Component.h"
#include "../../../Component/ComponentManager.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Imgui/imgui.h"
#include "../../../Imgui/imgui_impl_dx11.h"
#include "../../../Imgui/imgui_impl_win32.h"
#include "../../../Input/Input.h"
#include "../../../Math/Math.h"
#include "../../../System/Window.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"
#include "../../../Utility/LevelLoader.h"
#include <string>

ImGuiInspector::ImGuiInspector()
    : FileOperator("ImGuiInspector")
    , mInspectorPositionX(0.f)
{
}

ImGuiInspector::~ImGuiInspector() = default;

void ImGuiInspector::setTarget(const std::shared_ptr<GameObject>& target) {
    mTarget = target;
}

float ImGuiInspector::getInspectorPositionX() const {
    return mInspectorPositionX;
}

void ImGuiInspector::drawInspect() const {
    const auto& target = mTarget.lock();
    if (!target) {
        return;
    }

    //マウス位置を補正する
    const auto& mousePos = Input::mouse().getMousePosition();
    ImGui::GetIO().MousePos = ImVec2(mousePos.x, mousePos.y);
    //ウィンドウ位置を固定
    ImGui::SetNextWindowPos(ImVec2(mInspectorPositionX, 0.f), ImGuiCond_Always);
    //ウィンドウサイズを固定
    ImGui::SetNextWindowSize(ImVec2(Window::standardWidth() - mInspectorPositionX, Window::standardHeight()), ImGuiCond_Always);
    //ウィンドウカラーを設定
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.f, 0.f, 1.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.f, 0.f, 1.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.3f));

    ImGui::Begin("Inspector");

    drawName(*target);
    ImGui::Separator(); //区切り線
    drawTag(*target);
    ImGui::Separator(); //区切り線
    ImGui::Text("Transform");
    target->transform().drawInspector();
    ImGui::Separator(); //区切り線

    //全コンポーネントの情報を表示
    const auto& compList = target->componentManager().getAllComponents();
    for (const auto& comp : compList) {
        //最初はすべてのコンポーネントを開いておく
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
        //コンポーネントごとに階層を作る
        if (ImGui::TreeNode(comp->getComponentName().c_str())) {
            comp->drawInspector();

            ImGui::TreePop();
        }
    }

    //ゲームオブジェクト保存ボタン
    if (ImGui::Button("Save")) {
        LevelLoader::saveGameObject(*target);
    }

    ImGui::End();
    ImGui::PopStyleColor(3);
}

void ImGuiInspector::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mInspectorPositionX, "inspectorPositionX", inObj, alloc, mode);
}

void ImGuiInspector::drawName(const GameObject& target) const {
    const auto name = "Name: " + target.name();
    ImGui::Text(name.c_str());
}

void ImGuiInspector::drawTag(const GameObject& target) const {
    const auto tag = "Tag: " + target.tag();
    ImGui::Text(tag.c_str());
}
