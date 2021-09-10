#include "CollideAdder.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"

CollideAdder::CollideAdder() :
    Component() {
}

CollideAdder::~CollideAdder() = default;

void CollideAdder::addAABBCollide(MeshComponent& mesh) {
    mesh.addComponent<AABBCollider>("AABBCollider");
}
