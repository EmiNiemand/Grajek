//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/PlayerManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Camera.h"
#include "LowLevelClasses/GameData.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/Scripts/PlayerInput.h"
#include "Components/Scripts/PlayerMovement.h"
#include "Components/Scripts/PlayerEquipment.h"
#include "Components/Scripts/PlayerUI.h"
#include "Components/Scripts/SessionUI.h"
#include "Components/Scripts/MusicSession.h"
#include "Components/Scripts/SessionStarter.h"
#include "Components/Scripts/PauseMenu.h"
#include "Components/Scripts/OptionsMenu.h"
#include "Components/Scripts/ShopMenu.h"
#include "Components/UI/Button.h"
#include "Components/Animations/UIAnimator.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

PlayerManager::PlayerManager(const std::shared_ptr<GameObject> &parent, int id)
                            : Component(parent, id) {}

void PlayerManager::Awake() {
    movement = parent->AddComponent<PlayerMovement>();
    equipment = parent->AddComponent<PlayerEquipment>();
    equipment->Setup(5, 10);
    playerUI = GameObject::Instantiate("PlayerUI", parent)->AddComponent<PlayerUI>();
    playerUI->UpdateCash(equipment->GetCash());
    playerUI->UpdateRep(equipment->GetRep());
    sessionStarterUI = GameObject::Instantiate("SessionStarterUI", parent);
    GameObject::Instantiate("SessionUI", parent);

    moveInput = glm::vec2(0);
    inputEnabled = true;

    pauseMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>();
    optionsMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>();
    shopMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Shop")->GetComponent<ShopMenu>();
    activeMenu = nullptr;

    BuyInstrument(0, Prefab::GetInstrument(InstrumentName::Clap));
    Component::Awake();
}

void PlayerManager::Start() {
    Component::Start();
}

void PlayerManager::Update() {
#ifdef DEBUG
    ZoneScopedNC("Player manager", 0x800080);
#endif
    PollInput();
    Component::Update();
}

#pragma region Equipment events
bool PlayerManager::BuyInstrument(int price, const std::shared_ptr<Instrument> &instrument) {
    if(!equipment->BuyInstrument(price, instrument)) return false;

    playerUI->UpdateCash(equipment->GetCash());
    return true;
}

std::set<InstrumentName> PlayerManager::GetInstruments() {
    return equipment->GetInstrumentNames();
}
#pragma endregion

#pragma region Movement Events
void PlayerManager::OnMove(glm::vec2 moveVector) {
    if(moveVector != glm::vec2(0))
        moveVector = glm::normalize(moveVector);
	movement->Move(moveVector);
}
#pragma endregion

#pragma region Interaction Events
//TODO: implement interaction with IUsable
void PlayerManager::OnInteract() {
    if(session) return;
    if(activeMenu && activeMenu != shopMenu) return;

    if (!shopMenu->GetParent()->GetEnabled()) {
        GloomEngine::GetInstance()->timeScale = 0;
        shopMenu->ShowMenu();
        activeMenu = shopMenu;
    } else {
        GloomEngine::GetInstance()->timeScale = 1;
        shopMenu->HideMenu();
        activeMenu.reset();
    }
}
#pragma endregion

#pragma region UI Events
void PlayerManager::ToggleOptionsMenu() {
    if (activeMenu == pauseMenu) {
        pauseMenu->HideMenu();
        activeMenu = optionsMenu;
        optionsMenu->ShowMenu();
    }
    else if(activeMenu == optionsMenu) {
        optionsMenu->HideMenu();
        activeMenu = pauseMenu;
        pauseMenu->ShowMenu();
    }
}

void PlayerManager::OnMenuToggle() {
    if(session) return;
    if(activeMenu &&
        !(activeMenu == pauseMenu ||
          activeMenu == optionsMenu ||
          activeMenu == shopMenu)) return;

    if (activeMenu != shopMenu && activeMenu != pauseMenu) {
        GloomEngine::GetInstance()->timeScale = 0;
        if(activeMenu == optionsMenu)
            optionsMenu->HideMenu();
        pauseMenu->ShowMenu();
        activeMenu = pauseMenu;
    }
    else if(activeMenu == optionsMenu)
    {
        optionsMenu->HideMenu();
        pauseMenu->ShowMenu();
        activeMenu = pauseMenu;
    }
    else if(activeMenu == shopMenu) {
        GloomEngine::GetInstance()->timeScale = 1;
        shopMenu->HideMenu();
        activeMenu.reset();
    }
    else {
        GloomEngine::GetInstance()->timeScale = 1;
        pauseMenu->HideMenu();
        activeMenu.reset();
    }
}

void PlayerManager::OnApply() {
    if(!activeMenu) return;
    activeMenu->OnClick();
}

void PlayerManager::OnUIMove(glm::vec2 moveVector) {
    if(!activeMenu) return;
    activeMenu->ChangeActiveButton(moveVector);
}

#pragma endregion

#pragma region Music Session Events

void PlayerManager::OnSessionToggle() {
    if(activeMenu && activeMenu != sessionStarter) return;
    if (session) {
        session->Stop();
        session.reset();
        return;
    }
    if (sessionStarter) {
        sessionStarter->Stop();
        sessionStarter.reset();
        activeMenu.reset();
        GloomEngine::GetInstance()->timeScale = 1;
        return;
    }

    GloomEngine::GetInstance()->timeScale = 0;
    sessionStarter = GameObject::Instantiate("SessionStarter", sessionStarterUI)->AddComponent<SessionStarter>();
    activeMenu = sessionStarter;
    sessionStarter->Setup(equipment->instruments);
}

void PlayerManager::OnSoundPlay(int index) {
    if(!session) return;

    session->PlaySample(index);
}

void PlayerManager::OnSoundStop(int index) {
    if(!session) return;

    session->StopSample(index);
}

void PlayerManager::PlayedPattern(const std::shared_ptr<MusicPattern> &pat) {
    //TODO: uncomment when crowd manager gets implemented
//        crowdManager.PlayedPattern(pat);

    if (!pat) return;

    //TODO: uncomment when crowd manager gets implemented
    equipment->AddReward(1 /*crowdManager->GetCrowdSatisfaction()/100*/);

    playerUI->UpdateCash(equipment->cash);
    playerUI->UpdateRep(equipment->rep);
}

void PlayerManager::CreateMusicSession(InstrumentName instrument) {
    GloomEngine::GetInstance()->timeScale = 1;
    sessionStarter->Stop();
    sessionStarter.reset();
    activeMenu.reset();
    session = parent->AddComponent<MusicSession>();
    session->Setup(equipment->GetInstrumentWithName(instrument));
}

void PlayerManager::OnCheatSheetToggle() {
    if (!session) return;
    if (GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheetAnimator")) return;
    cheatSheetActive = !cheatSheetActive;
    if (cheatSheetActive) {
        GameObject::Instantiate("CheatSheetAnimator")->AddComponent<UIAnimator>()->Setup(
                GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheet")->GetComponent<Image>(), {
                        {AnimatedProperty::Position, glm::vec3(451.0f, -50.0f, 0.0f)}
                }, false);
    } else {
        GameObject::Instantiate("CheatSheetAnimator")->AddComponent<UIAnimator>()->Setup(
                GloomEngine::GetInstance()->FindGameObjectWithName("CheatSheet")->GetComponent<Image>(), {
                        {AnimatedProperty::Position, glm::vec3(451.0f, -1100.0f, 0.0f)}
                }, false);
    }
}

#pragma endregion

void PlayerManager::PollInput() {
	if(!inputEnabled) return;

	auto hid = HIDManager::GetInstance();
	glm::vec2 readMoveVector(0);

    // UI Input
    // --------
	for (auto key : PlayerInput::Menu)
		if(hid->IsKeyDown(key.first)) OnMenuToggle();

    for (auto key : PlayerInput::Interact)
        if(hid->IsKeyDown(key.first)) OnInteract();

    for (auto key: PlayerInput::StartSession)
        if (hid->IsKeyDown(key.first)) OnSessionToggle();

	if(activeMenu) {
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

    if(session) {
        for (auto key: PlayerInput::CheatSheet)
            if (hid->IsKeyDown(key.first)) OnCheatSheetToggle();

        for (auto key: PlayerInput::PlaySound) {
            if (hid->IsKeyDown(key.first)) OnSoundPlay(key.second);
            if (hid->IsKeyUp(key.first)) OnSoundStop(key.second);
        }
        return;
    }

	for (auto key: PlayerInput::Move) {
		if (hid->IsKeyPressed(key.first)) {
			readMoveVector.y = key.second == 0 ? 1 : key.second == 2 ? -1 : readMoveVector.y;
			readMoveVector.x = key.second == 1 ? 1 : key.second == 3 ? -1 : readMoveVector.x;
		}
	}

	if(readMoveVector != moveInput)
		OnMove(readMoveVector);
	moveInput = readMoveVector;
}

void PlayerManager::LoadData(std::shared_ptr<GameData> data) {
    equipment->Setup(data->money, data->reputation);
    playerUI->UpdateCash(data->money);
    playerUI->UpdateRep(data->reputation);
    parent->transform->SetLocalPosition(data->playerPosition);
    Camera::activeCamera->transform->SetLocalPosition(
            parent->transform->GetGlobalPosition() + Camera::activeCamera->GetComponent<Camera>()->cameraOffset);
    for(const auto& instrument : data->instruments)
        equipment->BuyInstrument(0, Prefab::GetInstrument(instrument));
}

void PlayerManager::SaveData(std::shared_ptr<GameData> &data) {
    data->money = equipment->GetCash();
    data->reputation = equipment->GetRep();
    data->playerPosition = parent->transform->GetLocalPosition();
    for(const auto& instrument : equipment->instruments)
        data->instruments.insert(instrument->name);
}
