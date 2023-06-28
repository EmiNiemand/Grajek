//
// Created by masterktos on 30.03.23.
//

#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Scripts/Player/PlayerInput.h"
#include "Components/Scripts/Player/PlayerMovement.h"
#include "Components/Scripts/Player/PlayerEquipment.h"
#include "Components/Scripts/Player/PlayerUI.h"
#include "Components/Scripts/Opponent.h"
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
#include "EngineManagers/DialogueManager.h"
#include "EngineManagers/SavePointManager.h"
#include "LowLevelClasses/GameData.h"
#include "GloomEngine.h"
#include "Components/Scripts/Menus/OptionsChooseMenu.h"
#include "EngineManagers/AudioManager.h"

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
    listener = AudioManager::GetInstance()->audioListener;
    rb = parent->AddComponent<Rigidbody>();

    // Add Player scripts
    // ------------------
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
    equipment->cash = 0;
    BuyInstrument(0, Instrument::GetInstrument(InstrumentName::Clap));

    // Set up Player's UI
    // ------------------
    playerUI->UpdateCash(equipment->GetCash(), false);
    playerUI->UpdateBadges(equipment->badges);

    // Set up Music Session
    // --------------------
    sessionStarterUI = GameObject::Instantiate("SessionStarterUI", parent);

    // Set up UI's
    // -----------
    // Note: You need to instantiate them beforehand, so do not move
    //       PlayerUI (or wherever instantiating gets moved) creation
    //       below these.
    pauseMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>();
    optionsMenu = GloomEngine::GetInstance()->FindGameObjectWithName("OptionsMenu")->GetComponent<OptionsMenu>();
    shopMenu = GloomEngine::GetInstance()->FindGameObjectWithName("ShopMenu")->GetComponent<ShopMenu>();
    savePointMenu = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenu")->GetComponent<SavePointMenu>();
    activeMenu = nullptr;

    sessionOpponent = nullptr;

    // Audio
    audioSource = parent->AddComponent<AudioSource>();
    audioSource->LoadAudioData("res/sounds/direct/walking_step_fast.wav", AudioType::Direct);
    audioSource->SetPitch(1.05f);
    audioSource->SetGain(0.25);
    audioSource->IsLooping(true);

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
    UpdateAnimationsAndSounds();
    listener->UpdatePosition(parent->transform->GetLocalPosition());

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

int PlayerManager::GetCash() {
    return equipment->cash;
}
#pragma endregion

#pragma region Animation, Sound Events
void PlayerManager::UpdateAnimationsAndSounds() {
    if (GloomEngine::GetInstance()->timeScale == 0) audioSource->StopSound();
	if(!rb) return;
    if(session || sessionStarter) return;

	float velocity = glm::length(glm::vec2(rb->velocity.x, rb->velocity.z));
	if (velocity > 0.01 && previousVelocity <= 0.01) {
        animator->SetAnimation("MainHero/MainHeroRun.dae");
		animator->speed = 2;
        audioSource->PlaySound();
	}
	else if (velocity <= 0.01 && previousVelocity > 0.01){
		animator->SetAnimation("MainHero/MainHeroIdle.dae");
        audioSource->StopSound();
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
void PlayerManager::OnInteract() {
    if(session || sessionStarter) return;
    if(activeMenu &&
        activeMenu != shopMenu &&
        activeMenu != savePointMenu) return;

    if (shopMenu->ShowMenu()) {
        activeMenu = shopMenu;
        OnMove({0, 0});
        return;
    }

    if (savePointMenu->ShowMenu()) {
        activeMenu = savePointMenu;
        OnMove({0, 0});
        return;
    }
}
#pragma endregion

#pragma region UI Events
void PlayerManager::ToggleOptionsMenu() {
    activeMenu->HideMenu();

    if (activeMenu == pauseMenu)
        activeMenu = optionsMenu;
    else if(activeMenu == optionsMenu) {
        activeMenu = pauseMenu;
        optionsMenu->chooseMenu->GetComponent<OptionsChooseMenu>()->ShowChooseMenu();
    }
    activeMenu->ShowMenu();
}

void PlayerManager::OnMenuToggle() {
    if(sessionStarter) {
        OnSessionToggle();
        return;
    }

    // Pause menu
    if(!activeMenu) {
        activeMenu = pauseMenu;
        activeMenu->ShowMenu();
        if (!session) {
            DialogueManager::GetInstance()->NotifyMenuIsActive();
            SavePointManager::GetInstance()->NotifyMenuIsNotActive();
        }
    }
    // Options -> Pause menu
    else if (activeMenu == optionsMenu) {
        ToggleOptionsMenu();
        if (!session)
            DialogueManager::GetInstance()->NotifyMenuIsActive();
    }
    // Disable any active menu
    else if(activeMenu) {
        activeMenu->HideMenu();
        activeMenu.reset();
        if (!session){
            SavePointManager::GetInstance()->NotifyMenuIsNotActive();
            DialogueManager::GetInstance()->NotifyMenuIsNotActive();
        }
    }
}

void PlayerManager::OnApply() {
    if(!(activeMenu || sessionStarter || session)) return;

    if(activeMenu) { activeMenu->OnClick(); return; }
    if(sessionStarter) { sessionStarter->OnClick(); return; }
    if(session) { session->OnClick(); return; }
}

void PlayerManager::OnUIMove(glm::vec2 moveVector) {
    if(!(activeMenu || sessionStarter || session)) return;

    if(activeMenu) { activeMenu->ChangeActiveButton(moveVector); return; }
    if(sessionStarter) { sessionStarter->ChangeActiveButton(moveVector); return; }
    if(session) { session->ChangeActiveButton(moveVector); return; }
}

#pragma endregion

#pragma region Music Session Events
void PlayerManager::OnSessionToggle() {
    // Cannot stop session if in duel
    if(sessionOpponent) return;
    if(activeMenu) return;

    auto dialogueManager = DialogueManager::GetInstance();
    auto savePointManager = SavePointManager::GetInstance();
    if(dialogueManager) dialogueManager->NotifyMenuIsNotActive();
    if(savePointManager) savePointManager->NotifyMenuIsNotActive();

    if (session) {
        SceneManager::GetInstance()->activeScene->GetComponent<AudioSource>()->SetGain(0.35f);
        Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(1.0f);
        session->Stop();
        session.reset();
        AIManager::GetInstance()->NotifyPlayerStopsPlaying();
        animator->LoadAnimationModel("MainHero/MainHeroIdle.dae");
        animator->SetAnimation("MainHero/MainHeroIdle.dae");
        animator->blend = true;
        // Needed to save session toggles
        OptionsManager::GetInstance()->Save();
        return;
    }
    if (sessionStarter) {
        SceneManager::GetInstance()->activeScene->GetComponent<AudioSource>()->SetGain(0.35f);
        sessionStarter->Stop();
        sessionStarter.reset();
        GloomEngine::GetInstance()->timeScale = 1;
        //TODO: ugly workaround
        float velocity = glm::length(glm::vec2(rb->velocity.x, rb->velocity.z));
        if(velocity > 0.01)
            audioSource->PlaySound();

        return;
    }

    if(dialogueManager) dialogueManager->NotifyMenuIsActive();
    if(savePointManager) savePointManager->NotifyMenuIsActive();
    SceneManager::GetInstance()->activeScene->GetComponent<AudioSource>()->SetGain(0.0f);
    GloomEngine::GetInstance()->timeScale = 0;
    //TODO: ugly workaround
    audioSource->StopSound();
    sessionStarter = GameObject::Instantiate("SessionStarter", sessionStarterUI)->AddComponent<SessionStarter>();
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

void PlayerManager::PlayedPattern(const std::shared_ptr<MusicPattern> &pat, float accuracy) {
     AIManager::GetInstance()->NotifyPlayerPlayedPattern(pat);

    if (!pat) return;

    if (sessionOpponent)
        sessionOpponent->PlayerPlayedPattern(AIManager::GetInstance()->
            GetPlayerSkillLevel(accuracy, (int) pat->sounds.size()));
    else
        equipment->AddReward(AIManager::GetInstance()->
            GetPlayerSkillLevel(accuracy, (int) pat->sounds.size()));

    playerUI->UpdateCash(equipment->cash);
}

void PlayerManager::CreateMusicSession(InstrumentName instrument) {
    OnMove(glm::vec2());
    Camera::activeCamera->GetComponent<Camera>()->SetZoomLevel(0.5f);
    GloomEngine::GetInstance()->timeScale = 1;
    sessionStarter->Stop();
    sessionStarter.reset();
    activeMenu.reset();
    auto chosenInstrument = equipment->GetInstrumentWithName(instrument);
    session = GameObject::Instantiate("SessionUI", parent)->AddComponent<MusicSession>();
    session->Setup(chosenInstrument);
    AIManager::GetInstance()->NotifyPlayerStartsPlaying(instrument, chosenInstrument->genre);
    animator->LoadAnimationModel("MainHero/MainHero"+chosenInstrument->NameToString()+".dae");

    // Attention: here blending gets disabled based on the fact that the OnSessionToggle() method
    //            (that enables it back) must be called at the end of session. Otherwise, blending
    //            will stay disabled through the rest of the game.
    animator->blend = false;
    animator->SetAnimation("MainHero/MainHero"+chosenInstrument->NameToString()+".dae");

    if (sessionOpponent)
        sessionOpponent->PlayerStartedMusicSession();
}

void PlayerManager::StartSessionWithOpponent(const std::shared_ptr<Opponent>& opponent) {
    if(!opponent) return;
    if(session) return;

    inputEnabled = true;
    OnSessionToggle();
    sessionOpponent = opponent;
}

void PlayerManager::EndSessionWithOpponent(bool wonSession, float moneyBet) {
    sessionOpponent = nullptr;
    OnSessionToggle();
    equipment->cash += moneyBet * (wonSession ? 1:-1);
    playerUI->UpdateCash(equipment->cash);
}

void PlayerManager::ReceiveBadge(PlayerBadges badge) {
    equipment->badges[badge] = true;
    playerUI->UpdateBadges(equipment->badges);

    auto badgePopUpAnimator = GameObject::Instantiate("BadgePopUpScaleAnimator", parent);
    auto badgePopUp = GameObject::Instantiate("BadgePopUp", badgePopUpAnimator)->AddComponent<Image>();
    std::string badgeName;
    switch (badge) {
        case PlayerBadges::DRUMS:
            badgeName = "Drums"; break;
        case PlayerBadges::TRUMPET:
            badgeName = "Trumpet"; break;
        case PlayerBadges::LAUNCHPAD:
            badgeName = "Launchpad"; break;
        case PlayerBadges::GUITAR:
            badgeName = "Guitar"; break;
    }

    badgePopUp->LoadTexture(920, 540, "UI/BadgePopUp/"+badgeName+".png", -0.99);
    badgePopUp->pivot = {0.5, 0.5};

    GameObject::Instantiate("BadgePopUpScaleAnimator", parent)->AddComponent<UIAnimator>()
            ->Setup(badgePopUp, {
                    {AnimatedProperty::Alpha, glm::vec3(0.0f), 0},
                    {AnimatedProperty::Alpha, glm::vec3(1.0f), 0.75},
                    {AnimatedProperty::Alpha, glm::vec3(1.0f), 1.0},
                    {AnimatedProperty::Alpha, glm::vec3(0.0f), 0.35}
            }, AnimationBehaviour::OneTime);
    badgePopUpAnimator->AddComponent<UIAnimator>()
        ->Setup(badgePopUp, {
                {AnimatedProperty::Scale, glm::vec3(0.0f), 0},
                {AnimatedProperty::Scale, glm::vec3(1.2f), 1},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.15},
                {AnimatedProperty::Scale, glm::vec3(1.1f), 0.15},
                {AnimatedProperty::Scale, glm::vec3(0.9f), 0.15},
                {AnimatedProperty::Scale, glm::vec3(1.0f), 0.15},
            {AnimatedProperty::Scale, glm::vec3(0.0f), 0.65}
    }, AnimationBehaviour::OneTime);
}

void PlayerManager::OnCheatSheetToggle() {
    if (!session) return;
    session->ToggleCheatSheet();
}

void PlayerManager::OnInstrumentControlToggle() {
    if (!session) return;
    session->ToggleInstrumentControl();
}

void PlayerManager::OnMetronomeSoundToggle() {
    if (!session) return;
    session->ToggleMetronomeSound();
}

void PlayerManager::OnMetronomeVisualsToggle() {
    if (!session) return;
    session->ToggleMetronomeVisuals();
}

void PlayerManager::OnBackingTrackToggle() {
    if (!session) return;
    session->ToggleBackingTrack();
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

	if(activeMenu || sessionStarter || session) {
        if(session) {
            for (auto key: PlayerInput::PlaySound) {
                if (hid->IsKeyDown(key.first)) OnSoundPlay(key.second);
                if (hid->IsKeyUp(key.first)) OnSoundStop(key.second);
            }
            for (auto key: PlayerInput::InstrumentControl)
                if (hid->IsKeyDown(key.first)) OnInstrumentControlToggle();
            for (auto key: PlayerInput::CheatSheet)
                if (hid->IsKeyDown(key.first)) OnCheatSheetToggle();
            for (auto key: PlayerInput::MetronomeSoundToggle)
                if (hid->IsKeyDown(key.first)) OnMetronomeSoundToggle();
            for (auto key: PlayerInput::MetronomeVisualToggle)
                if (hid->IsKeyDown(key.first)) OnMetronomeVisualsToggle();
            for (auto key: PlayerInput::BackingTrackToggle)
                if (hid->IsKeyDown(key.first)) OnBackingTrackToggle();
        }
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

	if(readMoveVector != moveInput)
		OnMove(readMoveVector);
	moveInput = readMoveVector;
}

void PlayerManager::LoadData(std::shared_ptr<GameData> data) {
    equipment->cash = data->money;

    for (auto badge : data->badges) {
        equipment->badges.at(badge.first) = badge.second;
    }

    playerUI->UpdateCash(data->money, false);
    playerUI->UpdateBadges(equipment->badges);
    parent->transform->SetLocalPosition(data->playerPosition);
    parent->UpdateSelfAndChildren();
    Camera::activeCamera->transform->SetLocalPosition(
            parent->transform->GetGlobalPosition() + Camera::activeCamera->GetComponent<Camera>()->cameraOffset);
    for(const auto& instrument : data->instruments)
        equipment->BuyInstrument(0, Instrument::GetInstrument(instrument));
}

void PlayerManager::SaveData(std::shared_ptr<GameData> &data) {
    data->money = equipment->GetCash();

    for (auto badge : equipment->badges) {
        data->badges.at(badge.first) = badge.second;
    }

    data->playerPosition = parent->transform->GetLocalPosition();
    for(const auto& instrument : equipment->instruments)
        data->instruments.insert(instrument->name);
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
    audioSource.reset();
    Component::OnDestroy();
}