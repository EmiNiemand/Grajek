//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/PlayerManager.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/PlayerInput.h"
#include "Components/Scripts/PlayerMovement.h"
#include "Components/Scripts/PlayerEquipment.h"
#include "Components/Scripts/PlayerUI.h"
#include "spdlog/spdlog.h"

PlayerManager::PlayerManager(const std::shared_ptr<GameObject> &parent, int id)
                            : Component(parent, id) {}

void PlayerManager::Start() {
    Component::Start();
    movement = parent->AddComponent<PlayerMovement>();
    equipment = parent->AddComponent<PlayerEquipment>();
    playerUI = GameObject::Instantiate("PlayerUI", parent)->AddComponent<PlayerUI>();

    // Temporary instrument, delete later
    std::vector<std::shared_ptr<Sample>> samples { std::make_shared<Sample>(), std::make_shared<Sample>(), std::make_shared<Sample>() };
    for (int i = 0; i < 3; ++i) samples[i]->id = i;
    auto pattern = std::make_shared<MusicPattern>();
    pattern->instrumentName = InstrumentName::Clap;
    pattern->sounds.push_back(std::make_shared<Sound>(samples[0], 0));
    pattern->sounds.push_back(std::make_shared<Sound>(samples[1], 1));
    pattern->sounds.push_back(std::make_shared<Sound>(samples[1], 1));

    auto instrument = std::make_shared<Instrument>();
    instrument->Setup(InstrumentName::Clap);
    instrument->samples = samples;
    instrument->patterns.push_back(pattern);

    session = parent->AddComponent<MusicSession>();
    session->Setup(instrument);

    moveInput = glm::vec2(0);
    inputEnabled = true;
	uiActive = false;
}

void PlayerManager::Update() {
    Component::Update();
    PollInput();
}

#pragma region Equipment events
bool PlayerManager::BuyInstrument(int price, const std::shared_ptr<Instrument> &instrument) {
    if(!equipment->BuyInstrument(price, instrument)) return false;

    playerUI->UpdateCash(equipment->GetCash());
    return true;
}
#pragma endregion

#pragma region Movement Events
void PlayerManager::OnMove(glm::vec2 moveVector) {
	movement->Move(glm::normalize(moveVector));
}
#pragma endregion

#pragma region Interaction Events
void PlayerManager::OnInteract() {
	//TODO: Place to plug everything up for Kamil
    spdlog::info("[PM] Interacting!");
}
#pragma endregion

#pragma region UI Events
void PlayerManager::OnMenuToggle() {
	//TODO: Place to plug everything up for Kamil
	uiActive = !uiActive;
    if (uiActive) {
        GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->EnableSelfAndChildren();
    } else {
        GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->DisableSelfAndChildren();
    }
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
#pragma endregion

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

    //if(session)
    for (auto key: PlayerInput::PlaySound) {
        if (hid->IsKeyDown(key.first)) {
            OnSoundPlay(key.second);
        }
    }

	if(readMoveVector != moveInput)
		OnMove(readMoveVector);
	moveInput = readMoveVector;
}

void PlayerManager::OnSoundPlay(int index) {
    spdlog::info("[PM] Played sound "+std::to_string(index)+"!");

    session->PlaySample(index);
}
