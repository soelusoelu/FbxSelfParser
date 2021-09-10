#include "SaveThis.h"
#include "../../../Utility/LevelLoader.h"

SaveThis::SaveThis()
    : Component()
{
}

SaveThis::~SaveThis() = default;

void SaveThis::finalize() {
    LevelLoader::saveGameObject(gameObject());
}
