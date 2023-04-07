//
// Created by szymo on 16/03/2023.
//

#include <numbers>
#include "Components/Scripts/PlayerMovement.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

PlayerMovement::PlayerMovement(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

PlayerMovement::~PlayerMovement() {}

void PlayerMovement::Start() {
    rb = parent->GetComponent<Rigidbody>();
    Component::Start();
}

void PlayerMovement::FixedUpdate() {
    if (rb != nullptr) {
        speed = std::lerp(speed, maxSpeed, smoothingParam);

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_W)) {
            rb->AddForce(glm::vec3(0, 0, -1) * speed, ForceMode::Force);
            horizontal = 1.0f;
            isMoving = true;
        }

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_S)) {
            rb->AddForce(glm::vec3(0, 0, 1) * speed, ForceMode::Force);
            horizontal = -1.0f;
            isMoving = true;
        }

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_A)) {
            rb->AddForce(glm::vec3(-1, 0, 0) * speed, ForceMode::Force);
            vertical = 1.0f;
            isMoving = true;
        }

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_D)) {
            rb->AddForce(glm::vec3(1, 0, 0) * speed, ForceMode::Force);
            vertical = -1.0f;
            isMoving = true;
        }

        if (!isMoving) {
            speed = 0.0f;
        } else {
            // Calculate rotation angles by using tangent function
            rotationAngle = atan2f(vertical, horizontal) * 180.0f/std::numbers::pi;

            if (rotationAngle < 0.0f) {
                rotationAngle += 360.0f;
            }

            rb->AddTorque(rotationAngle, ForceMode::Force);
        }

        horizontal = 0.0f;
        vertical = 0.0f;
        isMoving = false;
        Component::Update();
    }
}

void PlayerMovement::Move(glm::vec2 inputVector) {
    // Need to invert horizontal/Z axis
    moveVector = inputVector * glm::vec2(1, -1);
}
