#include "MapEditor.h"
#include "../CollideOperation/CollideMouseOperator.h"
#include "../Other/GameObjectSaveAndLoader.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectFactory.h"

MapEditor::MapEditor()
    : Scene()
{
}

MapEditor::~MapEditor() = default;

void MapEditor::awake() {
    const auto& g = GameObjectCreater::create("Ground1");
    const auto& saveLoader = g->componentManager().getComponent<GameObjectSaveAndLoader>();
    const auto& co = GameObjectCreater::create("CollideOperator");
    const auto& cmo = co->componentManager().getComponent<CollideMouseOperator>();
    cmo->setSaveAndLoader(saveLoader);
}

void MapEditor::update() {
    Debug::renderLine(Vector3::left * 100.f, Vector3::right * 100.f, ColorPalette::red);
    Debug::renderLine(Vector3::down * 100.f, Vector3::up * 100.f, ColorPalette::green);
    Debug::renderLine(Vector3::back * 100.f, Vector3::forward * 100.f, ColorPalette::blue);
}
