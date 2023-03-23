//
// Created by szymo on 16/03/2023.
//

#include "Components/Scripts/PlayerMovement.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

PlayerMovement::PlayerMovement(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent,
                               int id) : Component(gloomEngine, parent, id) {}

PlayerMovement::~PlayerMovement() {}

void PlayerMovement::Start() {
    rb = parent->GetComponent<Rigidbody>();
    Component::Start();
}

void PlayerMovement::Update() {
    if (gloomEngine->hidManager->IsKeyPressed(Key::KEY_W)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(0, 0, -1) * speed, ForceMode::Impulse);
        }
    }
    if (gloomEngine->hidManager->IsKeyPressed(Key::KEY_S)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(0, 0, 1) * speed, ForceMode::Impulse);
        }
    }
    if (gloomEngine->hidManager->IsKeyPressed(Key::KEY_A)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(-1, 0, 0) * speed, ForceMode::Impulse);
        }
    }
    if (gloomEngine->hidManager->IsKeyPressed(Key::KEY_D)) {
        if (rb != nullptr) {
            rb->AddForce(glm::vec3(1, 0, 0) * speed, ForceMode::Impulse);
        }
    }
    Component::Update();
}
