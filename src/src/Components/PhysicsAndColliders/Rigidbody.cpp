#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

Rigidbody::Rigidbody(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {
    parent->AddComponent<BoxCollider>();
}

Rigidbody::~Rigidbody() = default;


void Rigidbody::FixedUpdate() {
    parent->transform->SetLocalPosition(parent->transform->GetLocalPosition() + velocity);
    this->AddForce(glm::vec3(0, -1, 0) * gravityScale, ForceMode::Force);
    this->AddForce(-velocity * linearDrag, ForceMode::Force);

    parent->transform->SetLocalRotation(rotation);
    Component::FixedUpdate();
}

void Rigidbody::AddForce(glm::vec3 vector, ForceMode forceMode) {
    if (forceMode == ForceMode::Force) {
        velocity += vector / mass * GloomEngine::GetInstance()->fixedDeltaTime;
    }
    else {
        velocity += vector / mass;
    }
}

void Rigidbody::AddTorque(float angle, ForceMode forceMode) {
    // TODO: improve rotation based on the distance (angle needed to rotate)

    if (rotation.y == 360.0f) {
        rotation.y = 0.0f;
    }

    if (forceMode == ForceMode::Force) {
        if (angle == 0.0f && rotation.y > 180.0f) {
            rotation.y = std::lerp(rotation.y, angle + 360.0f, GloomEngine::GetInstance()->fixedDeltaTime * turnSpeed);
        } else {
            rotation.y = std::lerp(rotation.y, angle, GloomEngine::GetInstance()->fixedDeltaTime * turnSpeed);
        }
    }
    else {
        rotation.y = angle / mass;
    }
}

