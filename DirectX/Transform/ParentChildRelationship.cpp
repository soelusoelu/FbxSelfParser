#include "ParentChildRelationship.h"
#include "Transform3D.h"
#include "../GameObject/GameObject.h"

ParentChildRelationship::ParentChildRelationship(Transform3D* transform)
    : mTransform(transform)
    , mParent(nullptr)
    , mEquipmentPart(nullptr)
{
}

ParentChildRelationship::~ParentChildRelationship() {
    //すべての子を削除する
    for (const auto& child : mChildren) {
        child->destroy();
    }
}

void ParentChildRelationship::addChild(const Child& child) {
    //子を追加する
    mChildren.emplace_back(child);
    //子の親を自身に設定する
    child->transform().getParentChildRelation().setParent(this);

    mCallbackBuildingParentChildRelationship();
}

void ParentChildRelationship::removeChild(const Child& child) {
    mChildren.remove(child);
}

const std::list<std::shared_ptr<GameObject>>& ParentChildRelationship::getChildren() const {
    return mChildren;
}

ParentChildRelationship* ParentChildRelationship::parent() const {
    return mParent;
}

ParentChildRelationship* ParentChildRelationship::root() const {
    auto root = mParent;
    while (root) {
        auto p = root->mParent;
        if (!p) {
            break;
        }
        root = p;
    }
    return root;
}

size_t ParentChildRelationship::getChildCount() const {
    return mChildren.size();
}

void ParentChildRelationship::setEquipment(const Child& equipment, const Matrix4* equipmentPart) {
    //装備を子に追加する
    addChild(equipment);
    //装備に装備部位を設定する
    equipment->transform().getParentChildRelation().mEquipmentPart = equipmentPart;
}

const Matrix4* ParentChildRelationship::getEquipmentPart() const {
    return mEquipmentPart;
}

Transform3D& ParentChildRelationship::transform() const {
    return *mTransform;
}

void ParentChildRelationship::callbackBuildingParentChildRelationship(const std::function<void()>& f) {
    mCallbackBuildingParentChildRelationship += f;
}

void ParentChildRelationship::setParent(const Parent& parent) {
    mParent = parent;

    mCallbackBuildingParentChildRelationship();
}
