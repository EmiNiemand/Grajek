//
// Created by szymo on 16/03/2023.
//

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

void PlayerMovement::Update() {
    if (rb != nullptr) {
        speed = std::lerp(speed, maxSpeed, smoothingParam);

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_W)) {
            rb->AddForce(glm::vec3(0, 0, -1) * speed, ForceMode::Force);
            isMoving = true;
        }

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_S)) {
            rb->AddForce(glm::vec3(0, 0, 1) * speed, ForceMode::Force);
            isMoving = true;
        }

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_A)) {
            rb->AddForce(glm::vec3(-1, 0, 0) * speed, ForceMode::Force);
            isMoving = true;
        }

        if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_D)) {
            rb->AddForce(glm::vec3(1, 0, 0) * speed, ForceMode::Force);
            isMoving = true;
        }

        if (!isMoving) {
            speed = 0.0f;
        }

        isMoving = false;
        Component::Update();
    }
}
