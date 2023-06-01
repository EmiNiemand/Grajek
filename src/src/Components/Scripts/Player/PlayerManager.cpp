//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Scripts/Player/PlayerInput.h"
#include "Components/Scripts/Player/PlayerMovement.h"
#include "Components/Scripts/Player/PlayerEquipment.h"
#include "Components/Scripts/Player/PlayerUI.h"
#include "Components/Scripts/Menus/PauseMenu.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Menus/ShopMenu.h"
#include "Components/Scripts/Menus/SavePointMenu.h"
#include "Components/Scripts/SessionUI/SessionUI.h"
#include "Components/Scripts/MusicSession.h"
#include "Components/Scripts/SessionStarter.h"
#include "Components/Scripts/Instrument.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Animator.h"
#include "Components/UI/Button.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Animations/UIAnimator.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "EngineManagers/HIDManager.h"
#include "EngineManagers/OptionsManager.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/DataPersistanceManager.h"
#include "EngineManagers/OpponentManager.h"
#include "EngineManagers/DialogueManager.h"
#include "EngineManagers/SavePointManager.h"
#include "LowLevelClasses/GameData.h"
#include "GloomEngine.h"

#include <filesystem>

#ifdef DEBUG
#include "tracy/Tracy.hpp"
#endif

PlayerManager::PlayerManager(const std::shared_ptr<GameObject> &parent, int id)
                            : Component(parent, id) {}

void PlayerManager::Awake() {
    moveInput = glm::vec2(0);
    inputEnabled = true;

    // Add Components
    // --------------
    parent->AddComponent<AudioListener>();
    rb = parent->AddComponent<Rigidbody>();

    // Add Player scripts
    // ------------------sssss
    movement = parent->AddComponent<PlayerMovement>();
    equipment = parent->AddComponent<PlayerEquipment>();
    playerUI = GameObject::Instantiate("PlayerUI", parent)->AddComponent<PlayerUI>();

    // Set up Model Animator
    //----------------------
    auto animatorObject = GameObject::Instantiate("Animator", parent);
    animatorObject->transform->SetLocalRotation({0, 180, 0});
    animator = animatorObject->AddComponent<Animator>();
    animator->LoadAnimationModel("MainHero/MainHeroIdle.dae");
    animator->SetAnimation("MainHero/MainHeroIdle.dae");

    // Set up Collider
    //----------------
    auto collider = parent->GetComponent<BoxCollider>();
    collider->SetOffset({0, 1.5, 0});
    collider->SetSize({1, 2, 1});
    collider->isDynamic = true;

    // Set up Equipment
    // ----------------
    equipment->Setup(0, 0);
    BuyInstrument(0, Instrument::GetInstrument(InstrumentName::Clap));

    // Set up Player's UI
    // ------------------
    playerUI->UpdateCash(equipment->GetCash(), false);

    // Set up Music Session
    // --------------------
    sessionStarterUI = GameObject::Instantiate("SessionStarterUI", parent);

    pauseMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>();
    optionsMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>();
    shopMenu = GloomEngine::GetInstance()->FindGameObjectWithName("ShopMenu")->GetComponent<ShopMenu>();
    savePointMenu = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenu")->GetComponent<SavePointMenu>();
    activeMenu = nullptr;

    // Load game
    // ---------
    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "Saves";

    DataPersistanceManager::GetInstance()->LoadGame(path.string(), SceneManager::GetInstance()->file);

    Component::Awake();
}

void PlayerManager::Start() { Component::Start(); }

void PlayerManager::Update() {
#ifdef DEBUG
    ZoneScopedNC("Player manager", 0x800080);
#endif
    PollInput();
	UpdateAnimations();

    Component::Update();
}

#pragma region Equipment Events
bool PlayerManager::BuyInstrument(int price, const std::shared_ptr<Instrument> &instrument) {
    if(!equipment->BuyInstrument(price, instrument)) return false;

    playerUI->UpdateCash(equipment->GetCash(), price!=0);
    return true;
}

std::set<InstrumentName> PlayerManager::GetInstruments() {
    return equipment->GetInstrumentNames();
}
#pragma endregion

#pragma region Animation Events
void PlayerManager::UpdateAnimations() {
	if(!rb) return;

	float velocity = glm::length(glm::vec2(rb->velocity.x, rb->velocity.z));
	if (velocity > 0.01 && previousVelocity <= 0.01) {
        animator->SetAnimation("MainHero/MainHeroRun.dae");
		animator->speed = 2;
	}
	else if (velocity <= 0.01 && previousVelocity > 0.01){
		animator->SetAnimation("MainHero/MainHeroIdle.dae");
	}
    previousVelocity = velocity;
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
    GloomEngine::GetInstance()->timeScale = 1;
    if (shopMenu->GetParent()->GetEnabled())
        shopMenu->HideMenu();
    if (savePointMenu->GetParent()->GetEnabled())
        savePointMenu->HideMenu();
    activeMenu.reset();
}
#pragma endregion

//TODO: rewrite these monstrosities
#pragma region UI Events
void PlayerManager::ToggleOptionsMenu() {
    //TODO: this should be simply controlled by pauseMenu
    // (which means that PlayerManager here only calls method)
    // in PauseMenu (or even activeMenu)
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
    //TODO: this all should be simply controlled by activeMenu alone
    //if(session) return;
    if(activeMenu &&
        !(activeMenu == pauseMenu ||
          activeMenu == optionsMenu ||
          activeMenu == shopMenu ||
          activeMenu == savePointMenu)) return;

    DialogueManager::GetInstance()->NotifyMenuIsActive();
    // Pause menu
    if(!activeMenu) {
        GloomEngine::GetInstance()->timeScale = 0;
        activeMenu = pauseMenu;
        pauseMenu->ShowMenu();
    }
    // Options -> Pause menu
    else if (activeMenu == optionsMenu) {
        OptionsManager::GetInstance()->Save();
        optionsMenu->HideMenu();
        pauseMenu->ShowMenu();
        activeMenu = pauseMenu;
    }
    // Disable any active menu
    else if(activeMenu) {
        GloomEngine::GetInstance()->timeScale = 1;
        activeMenu->HideMenu();
        activeMenu.reset();
        DialogueManager::GetInstance()->NotifyMenuIsNotActive();
    }
}

void PlayerManager::OnApply() {
    if(!(activeMenu || sessionStarter)) return;

    if(activeMenu) { activeMenu->OnClick(); return; }
    sessionStarter->OnClick();
}

void PlayerManager::OnUIMove(glm::vec2 moveVector) {
    if(!(activeMenu || sessionStarter)) return;

    if(sessionStarter) { sessionStarter->ChangeActiveButton(moveVector); return; }
    activeMenu->ChangeActiveButton(moveVector);
}

#pragma endregion

#pragma region Music Session Events
void PlayerManager::OnSessionToggle() {
    //TODO: this method implementation just seems wrong
    //if(activeMenu && activeMenu != sessionStarter) return;
    auto dialogueManager = DialogueManager::GetInstance();
    auto savePointManager = SavePointManager::GetInstance();
    if(dialogueManager) dialogueManager->NotifyMenuIsNotActive();
    if(savePointManager) savePointManager->NotifyMenuIsNotActive();

    if (session) {
        Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);
        session->Stop();
        session.reset();
        AIManager::GetInstance()->NotifyPlayerStopsPlaying();
        OpponentManager::GetInstance()->NotifyPlayerStopsPlaying();
        return;
    }
    if (sessionStarter) {
        sessionStarter->Stop();
        sessionStarter.reset();
        //activeMenu.reset();
        GloomEngine::GetInstance()->timeScale = 1;
        return;
    }

    if(dialogueManager) DialogueManager::GetInstance()->NotifyMenuIsActive();
    if(savePointManager) savePointManager->NotifyMenuIsActive();
    GloomEngine::GetInstance()->timeScale = 0;
    sessionStarter = GameObject::Instantiate("SessionStarter", sessionStarterUI)->AddComponent<SessionStarter>();
    //activeMenu = sessionStarter;
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
     OpponentManager::GetInstance()->NotifyPlayerPlayedPattern(AIManager::GetInstance()->GetCombinedPlayerSatisfaction());

    if (!pat) return;

    spdlog::info("Crowd satisfaction: "+std::to_string(AIManager::GetInstance()->GetCombinedPlayerSatisfaction()));
    equipment->AddReward(AIManager::GetInstance()->GetCombinedPlayerSatisfaction() / 100.0f);

    playerUI->UpdateCash(equipment->cash);
}

void PlayerManager::CreateMusicSession(InstrumentName instrument) {
    OnMove(glm::vec2());
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(0.5f);
    GloomEngine::GetInstance()->timeScale = 1;
    sessionStarter->Stop();
    sessionStarter.reset();
    activeMenu.reset();
    session = GameObject::Instantiate("SessionUI", parent)->AddComponent<MusicSession>();
    session->Setup(equipment->GetInstrumentWithName(instrument));
    AIManager::GetInstance()->NotifyPlayerStartsPlaying(instrument, equipment->GetInstrumentWithName(instrument)->genre);
}

void PlayerManager::OnCheatSheetToggle() {
    if (!session) return;
    session->ToggleCheatSheet();
}

void PlayerManager::OnPlayerLoseDuel() {
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);
    session->Stop();
    session.reset();
    AIManager::GetInstance()->NotifyPlayerStopsPlaying();
    OpponentManager::GetInstance()->NotifyPlayerStopsPlaying();
    DialogueManager::GetInstance()->NotifyMenuIsNotActive();
    // TODO add sound when player beat boss
}

void PlayerManager::OnDestroy() {
    session.reset();
    sessionStarter.reset();
    sessionStarterUI.reset();
    playerUI.reset();
    equipment.reset();
    movement.reset();
    pauseMenu.reset();
    optionsMenu.reset();
    shopMenu.reset();
    savePointMenu.reset();
    animator.reset();
    rb.reset();
    activeMenu.reset();
    Component::OnDestroy();
}

#pragma endregion

void PlayerManager::PollInput() {
	if(!inputEnabled) {
        if(moveInput != glm::vec2(0)) OnMove(glm::vec2(0));
        return;
    }

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

	if(activeMenu || sessionStarter) {
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
    playerUI->UpdateCash(data->money, false);
    parent->transform->SetLocalPosition(data->playerPosition);
    parent->UpdateSelfAndChildren();
    Camera::activeCamera->transform->SetLocalPosition(
            parent->transform->GetGlobalPosition() + Camera::activeCamera->GetComponent<Camera>()->cameraOffset);
    for(const auto& instrument : data->instruments)
        equipment->BuyInstrument(0, Instrument::GetInstrument(instrument));
}

void PlayerManager::SaveData(std::shared_ptr<GameData> &data) {
    data->money = equipment->GetCash();
    data->playerPosition = parent->transform->GetLocalPosition();
    for(const auto& instrument : equipment->instruments)
        data->instruments.insert(instrument->name);
}
