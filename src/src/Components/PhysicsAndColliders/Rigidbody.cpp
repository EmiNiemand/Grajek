#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

Rigidbody::Rigidbody(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent, int id)
        : Component(gloomEngine, parent, id) {
    parent->AddComponent<BoxCollider>();
}

Rigidbody::~Rigidbody() {}


void Rigidbody::Update() {
    parent->transform->SetLocalPosition(parent->transform->GetLocalPosition() + velocity);
    this->AddForce(glm::vec3(0, -1, 0) * gravityScale, ForceMode::Force);
    this->AddForce(-velocity * linearDrag, ForceMode::Force);
    Component::Update();
}

void Rigidbody::AddForce(glm::vec3 vector, ForceMode forceMode) {
    if (forceMode == ForceMode::Force) {
        velocity += vector / mass * gloomEngine->deltaTime;
    }
    else {
        velocity += vector / mass;
    }
}

