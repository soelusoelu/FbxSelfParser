#include "Component.h"

Component::Component() :
    mGameObjectGetter(nullptr),
    mComponentName("") {
}

Component::~Component() = default;

GameObject& Component::gameObject() const {
    return mGameObjectGetter->getThis();
}

Transform3D& Component::transform() const {
    return gameObject().transform();
}

const std::string& Component::getComponentName() const {
    return mComponentName;
}

ComponentManager& Component::componentManager() const {
    return gameObject().componentManager();
}
