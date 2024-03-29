//
// Created by szymo on 16/03/2023.
//

#include <numbers>
#include "Components/Scripts/Player/PlayerMovement.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

PlayerMovement::PlayerMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

PlayerMovement::~PlayerMovement() = default;

void PlayerMovement::Start() {
    rb = parent->GetComponent<Rigidbody>();
    Component::Start();
}

void PlayerMovement::OnDestroy() {
    rb.reset();
    Component::OnDestroy();
}

void PlayerMovement::FixedUpdate() {
    Component::FixedUpdate();
    if (rb == nullptr)
        return;

    speed = std::lerp(speed, maxSpeed, smoothingParam);

    if (moveVector.x != 0.0f || moveVector.y != 0.0f) {
        rb->AddForce(glm::vec3(moveVector.x, 0.0f, moveVector.y) * speed, ForceMode::Force);
        isMoving = true;
    }

    if (!isMoving) {
        speed = 0.0f;
    } else {
        // Calculate rotation angles by using tangent function
        rotationAngle = std::atan2f(-moveVector.x, -moveVector.y) * 180.0f/std::numbers::pi;

        rb->AddTorque(rotationAngle, ForceMode::Force);
    }

    isMoving = false;
}

void PlayerMovement::Move(glm::vec2 inputVector) {
    // Need to invert horizontal/Z axis
    moveVector = inputVector * glm::vec2(1, -1);
}