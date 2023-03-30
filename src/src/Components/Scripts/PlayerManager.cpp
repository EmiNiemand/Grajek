//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/PlayerManager.h"
#include "EngineManagers/HIDManager.h"
#include "Components/Scripts/PlayerInput.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "spdlog/spdlog.h"

PlayerManager::PlayerManager(const std::shared_ptr<GameObject> &parent, int id)
                            : Component(parent, id) {
    movement = parent->GetComponent<PlayerMovement>();
    inputEnabled = true;
//    equipment = parent->GetComponent<PlayerEquipment>();
}

void PlayerManager::Start() {
    Component::Start();
}

void PlayerManager::Update() {
    Component::Update();
    PollInput();
}

void PlayerManager::PollInput() {
    if(!inputEnabled) return;

    auto hid = HIDManager::GetInstance();
    glm::vec2 readMoveVector(0);

    for (auto key : PlayerInput::Move) {
        if(hid->IsKeyPressed(key.first)) {
            readMoveVector.y = key.second==0?1: key.second==2?-1 :readMoveVector.y;
            readMoveVector.x = key.second==1?1: key.second==3?-1 :readMoveVector.x;
        }
    }
    for (auto key : PlayerInput::Interact)
        if(hid->IsKeyDown(key.first)) OnInteract();

    if(readMoveVector != glm::vec2(0) || readMoveVector != moveVector)
        OnMove(readMoveVector);
    moveVector = readMoveVector;
}

void PlayerManager::OnMove(glm::vec2 moveVector) {
//    spdlog::info(std::to_string(moveVector.x) + " | " + std::to_string(moveVector.y));
}

void PlayerManager::OnInteract() {
    spdlog::info("[PM] Interacting!");
}
