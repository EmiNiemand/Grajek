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

void Rigidbody::AddTorque(float targetRotation, ForceMode forceMode) {
    if (targetRotation == 0.0f && rotation.y > 180.0f) {
        targetRotation = 360.0f;
    }

    if (rotation.y > 360.0f) {
        rotation.y -= 360.0f;
        targetRotation -= 360.f;
    }

    if ((targetRotation + 360.0f - rotation.y) <= (rotation.y - targetRotation)) {
        targetRotation += 360.0f;
    } else if ((rotation.y + 360.0f - targetRotation) < (targetRotation - rotation.y)) {
        rotation.y  += 360.0f;
    }

    if (forceMode == ForceMode::Force) {
        rotation.y = std::lerp(rotation.y, targetRotation,
                               GloomEngine::GetInstance()->fixedDeltaTime * turnSpeed);
    }
    else {
        rotation.y = std::lerp(rotation.y, targetRotation, turnSpeed);
    }
}

