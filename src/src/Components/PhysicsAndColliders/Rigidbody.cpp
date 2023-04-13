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
    if (angle == 0.0f && rotation.y > 180.0f) {
        angle = 360.0f;
    }

    if ((angle + 360.0f - rotation.y) < (rotation.y - angle)) {
        targetRotation.y = 360.0f + angle;
    } else {
        targetRotation.y = angle;
    }

    if (rotation.y > 360.0f) {
        rotation.y -= 360.0f;
        targetRotation.y -= 360.f;
    }

    spdlog::info(rotation.y );
    if (forceMode == ForceMode::Force) {
        rotation.y = std::lerp(rotation.y, targetRotation.y, GloomEngine::GetInstance()->fixedDeltaTime * turnSpeed);
    }
    else {
        rotation.y = std::lerp(rotation.y, targetRotation.y, turnSpeed);
    }
}

