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
	uiActive = false;
    equipment = parent->GetComponent<PlayerEquipment>();
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


    for (auto key : PlayerInput::Menu)
        if(hid->IsKeyDown(key.first)) OnMenuToggle();

	if(uiActive) {
		for (auto key: PlayerInput::Move) {
			if (hid->IsKeyDown(key.first)) {
				readMoveVector.y = key.second == 0 ? 1 : key.second == 2 ? -1 : readMoveVector.y;
				readMoveVector.x = key.second == 1 ? 1 : key.second == 3 ? -1 : readMoveVector.x;
			}
		}

        for (auto key : PlayerInput::Apply)
            if(hid->IsKeyDown(key.first)) OnApply();
        if(readMoveVector != glm::vec2(0))
            OnUIMove(readMoveVector);

        return;
	}

    for (auto key: PlayerInput::Move) {
        if (hid->IsKeyPressed(key.first)) {
            readMoveVector.y = key.second == 0 ? 1 : key.second == 2 ? -1 : readMoveVector.y;
            readMoveVector.x = key.second == 1 ? 1 : key.second == 3 ? -1 : readMoveVector.x;
        }
    }
    for (auto key : PlayerInput::Interact)
        if(hid->IsKeyDown(key.first)) OnInteract();

	for (auto key : PlayerInput::Save)
		if(hid->IsKeyDown(key.first)) OnSaveLoad(true);
	for (auto key : PlayerInput::Load)
		if(hid->IsKeyDown(key.first)) OnSaveLoad(false);

    if(readMoveVector != glm::vec2(0)/* || readMoveVector != moveVector*/)
        OnMove(readMoveVector);
    moveVector = readMoveVector;
}

void PlayerManager::OnMove(glm::vec2 moveVector) {
	movement->Move(moveVector);
}

void PlayerManager::OnInteract() {
	//TODO: Place to plug everything up for Kamil
    spdlog::info("[PM] Interacting!");
}

void PlayerManager::OnSaveLoad(bool save) {
	//TODO: Place to plug everything up for Szymon
	if(save)
	{
		spdlog::info("[PM] Saving game...");
	}
	else
	{
		spdlog::info("[PM] Loading game...");
	}
}

void PlayerManager::OnMenuToggle() {
	//TODO: Place to plug everything up for Kamil
	uiActive = !uiActive;
    spdlog::info("[PM] Menu" + std::string(uiActive?"enabled":"disabled") + "!");
}

void PlayerManager::OnApply() {
	if(!uiActive) return;

	//TODO: Place to plug everything up for Kamil
	spdlog::info("[PM] Applied some option in menu!");
}

void PlayerManager::OnUIMove(glm::vec2 moveVector) {
    //TODO: Place to plug everything up for Kamil
    spdlog::info("Moving inside UI!");
}

