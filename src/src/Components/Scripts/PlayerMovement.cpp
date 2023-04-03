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

void PlayerMovement::FixedUpdate() {
    if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_W)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(0, 0, -1) * speed, ForceMode::Impulse);
        }
    }
    if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_S)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(0, 0, 1) * speed, ForceMode::Impulse);
        }
    }
    if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_A)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(-1, 0, 0) * speed, ForceMode::Impulse);
        }
    }
    if (HIDManager::GetInstance()->IsKeyPressed(Key::KEY_D)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(1, 0, 0) * speed, ForceMode::Impulse);
        }
    }
    Component::FixedUpdate();
}

void PlayerMovement::Move(glm::vec2 moveVector) {
    if(!rb) return;
    //TODO: might need to normalize vector
    rb->AddForce(glm::vec3(moveVector.x, 0, moveVector.y) * speed, ForceMode::Impulse);
}
