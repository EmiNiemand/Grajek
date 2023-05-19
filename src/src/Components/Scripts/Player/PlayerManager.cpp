//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/Player/PlayerManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Camera.h"
#include "LowLevelClasses/GameData.h"
#include "EngineManagers/HIDManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/Scripts/Player/PlayerInput.h"
#include "Components/Scripts/Player/PlayerMovement.h"
#include "Components/Scripts/Player/PlayerEquipment.h"
#include "Components/Scripts/Player/PlayerUI.h"
#include "Components/Scripts/SessionUI/SessionUI.h"
#include "Components/Scripts/MusicSession.h"
#include "Components/Scripts/SessionStarter.h"
#include "Components/Scripts/Menus/PauseMenu.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Menus/ShopMenu.h"
#include "Components/Scripts/Menus/SavePointMenu.h"
#include "Components/UI/Button.h"
#include "Components/Animations/UIAnimator.h"
#include "EngineManagers/OptionsManager.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/DataPersistanceManager.h"
#include "Components/Renderers/Animator.h"

#include <filesystem>

#ifdef DEBUG
#include "tracy/Tracy.hpp"
#endif

PlayerManager::PlayerManager(const std::shared_ptr<GameObject> &parent, int id)
                            : Component(parent, id) {}

void PlayerManager::Awake() {
    movement = parent->AddComponent<PlayerMovement>();
    equipment = parent->AddComponent<PlayerEquipment>();
    auto animatorObject = GameObject::Instantiate("Animator", parent);
    animator = animatorObject->AddComponent<Animator>();
    // TODO: Change model later
    animator->LoadAnimationModel("AnimsNew/Walk.dae");
    animator->SetAnimation("AnimsNew/Idle1.dae");
    animatorObject->transform->SetLocalRotation({0, 180, 0});
    equipment->Setup(0, 0);
    playerUI = GameObject::Instantiate("PlayerUI", parent)->AddComponent<PlayerUI>();
    playerUI->UpdateCash(equipment->GetCash());
    playerUI->UpdateRep(equipment->GetRep());
    sessionStarterUI = GameObject::Instantiate("SessionStarterUI", parent);
    GameObject::Instantiate("SessionUI", parent);

    moveInput = glm::vec2(0);
    inputEnabled = true;

    pauseMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>();
    optionsMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>();
    shopMenu = GloomEngine::GetInstance()->FindGameObjectWithName("ShopMenu")->GetComponent<ShopMenu>();
    savePointMenu = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenu")->GetComponent<SavePointMenu>();
    activeMenu = nullptr;

    BuyInstrument(0, Instrument::GetInstrument(InstrumentName::Clap));

    // Load game
    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "Saves";

    DataPersistanceManager::GetInstance()->LoadGame(path.string(), SceneManager::GetInstance()->file);

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
    if(moveVector != glm::vec2(0)) {
        moveVector = glm::normalize(moveVector);
        animator->SetAnimation("AnimsNew/Walk.dae");
    }
    else {
        animator->SetAnimation("AnimsNew/Idle1.dae");
    }

	movement->Move(moveVector);
}
#pragma endregion

#pragma region Interaction Events
//TODO: implement interaction with IUsable
void PlayerManager::OnInteract() {
    if(session) return;
    if(activeMenu && activeMenu != shopMenu && activeMenu != savePointMenu) return;

    if (!shopMenu->GetParent()->GetEnabled()) {
        if (shopMenu->ShowMenu()) {
            GloomEngine::GetInstance()->timeScale = 0;
            activeMenu = shopMenu;
            return;
        }
    }
    if (!savePointMenu->GetParent()->GetEnabled()) {
        if (savePointMenu->ShowMenu()) {
            GloomEngine::GetInstance()->timeScale = 0;
            activeMenu = savePointMenu;
            return;
        }
    }
    if (shopMenu->GetParent()->GetEnabled() || savePointMenu->GetParent()->GetEnabled()) {
        GloomEngine::GetInstance()->timeScale = 1;
        shopMenu->HideMenu();
        savePointMenu->HideMenu();
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
        if(activeMenu == optionsMenu) {
            OptionsManager::GetInstance()->Save();
            optionsMenu->HideMenu();
        }
        pauseMenu->ShowMenu();
        activeMenu = pauseMenu;
    }
    // TODO/INFO: this code isn't reached
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
        Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);
        session->Stop();
        session.reset();
        AIManager::GetInstance()->NotifyPlayerStopsPlaying();
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
     AIManager::GetInstance()->NotifyPlayerPlayedPattern(pat);

    if (!pat) return;

    //TODO: uncomment when crowd manager gets implemented
    spdlog::info("Crowd satisfaction: "+std::to_string(AIManager::GetInstance()->GetCombinedSatisfaction()));
    equipment->AddReward(AIManager::GetInstance()->GetCombinedSatisfaction()/100.0f);

    playerUI->UpdateCash(equipment->cash);
    playerUI->UpdateRep(equipment->rep);
}

void PlayerManager::CreateMusicSession(InstrumentName instrument) {
    OnMove(glm::vec2());
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(0.5f);
    GloomEngine::GetInstance()->timeScale = 1;
    sessionStarter->Stop();
    sessionStarter.reset();
    activeMenu.reset();
    session = parent->AddComponent<MusicSession>();
    session->Setup(equipment->GetInstrumentWithName(instrument));
    AIManager::GetInstance()->NotifyPlayerStartsPlaying(instrument, equipment->GetInstrumentWithName(instrument)->genre);
}

void PlayerManager::OnCheatSheetToggle() {
    if (!session) return;
    session->ToggleCheatSheet();
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
    parent->UpdateSelfAndChildren();
    Camera::activeCamera->transform->SetLocalPosition(
            parent->transform->GetGlobalPosition() + Camera::activeCamera->GetComponent<Camera>()->cameraOffset);
    for(const auto& instrument : data->instruments)
        equipment->BuyInstrument(0, Instrument::GetInstrument(instrument));
}

void PlayerManager::SaveData(std::shared_ptr<GameData> &data) {
    data->money = equipment->GetCash();
    data->reputation = equipment->GetRep();
    data->playerPosition = parent->transform->GetLocalPosition();
    for(const auto& instrument : equipment->instruments)
        data->instruments.insert(instrument->name);
}
