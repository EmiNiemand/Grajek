#include "Game.h"
#include "GloomEngine.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/CubeMap.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Scripts/Player/PlayerMovement.h"
#include "Components/Scripts/Menus/PauseMenu.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Renderers/Animator.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Scripts/Menus/SavePointMenu.h"
#include "Components/Animations/GameObjectAnimator.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "GameObjectsAndPrefabs/Prefabs/Player.h"
#include "GameObjectsAndPrefabs/Prefabs/Die.h"
#include "GameObjectsAndPrefabs/Prefabs/Shop.h"
#include "GameObjectsAndPrefabs/Prefabs/SavePoint.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Menus/Shopkeeper.h"
#include "EngineManagers/AIManager.h"
#include "Components/Scripts/Opponent.h"
#include "Components/Scripts/Instrument.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

Game::Game() {
    activeCamera = Camera::activeCamera;
    activeScene = SceneManager::GetInstance()->activeScene;
}

Game::~Game() = default;

void Game::InitializeGame() const {
#ifdef DEBUG
    ZoneScopedNC("Game Init", 0xDC143C);
#endif
    // Set up camera
    // -------------
    std::shared_ptr<Camera> camera = activeCamera->AddComponent<Camera>();
    camera->cameraOffset = glm::vec3(0, 30, 30);

    // Set up cubemap
    // --------------
    auto sky = GameObject::Instantiate("CubeMap", activeScene);
    auto skyCubeMap = sky->AddComponent<CubeMap>();
    skyCubeMap->LoadTextures("skybox/");

    // Load all animations
    // -------------
    Animator::LoadAnimation("CrowdAnimations/Walk.dae");
    Animator::LoadAnimation("CrowdAnimations/Happy.dae");
    Animator::LoadAnimation("CrowdAnimations/Angry.dae");
    Animator::LoadAnimation("CrowdAnimations/Idle1.dae");
    Animator::LoadAnimation("CrowdAnimations/Idle3.dae");
    Animator::LoadAnimation("MainHero/MainHeroIdle.dae");
    Animator::LoadAnimation("MainHero/MainHeroRun.dae");
    Animator::LoadAnimation("MainHero/MainHeroClap.dae");
    Animator::LoadAnimation("MainHero/MainHeroTrumpet.dae");
    Animator::LoadAnimation("MainHero/MainHeroDrums.dae");

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = Prefab::Instantiate<Player>();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::Instantiate<Die>("Ground");
    ground->transform->SetLocalPosition({0, -4, 0});
    ground->transform->SetLocalScale({150, 2, 150});
    ground->GetComponent<Renderer>()->textScale = glm::vec2(150, 150);

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({20, 40, 20});
    sun->transform->SetLocalRotation({-50, 70, 0});

    auto opponent = GameObject::Instantiate("JazzOpponent", activeScene);
    opponent->AddComponent<Renderer>()->LoadModel("Opponent/opponent.obj");
    opponent->AddComponent<BoxCollider>()->SetSize({3, 1, 3});
    opponent->transform->SetLocalPosition(glm::vec3(-87, 0, -42));
    opponent->transform->SetLocalRotation(glm::vec3(0, 45, 0));
    // 2      *   *
    // 1    *   *
    // 0  *
    auto opponentComponent = opponent->AddComponent<Opponent>();
    opponentComponent->Setup(Instrument::GetInstrument(InstrumentName::Drums),
                                              {{0, 0.5}, {1, 0.5}, {2, 0.5}, {1, 0.5}, {2, 0.5}}, 80.0f, 50);
    opponentComponent->dialogue->texts.push_back({{""},
                                                      {"Zaplac jezeli chcesz ze mna walczyc."},
                                                      {""}});
    opponentComponent->dialogue->texts.push_back({{""},
                                                  {"Walcz!."},
                                                  {""}});
    opponentComponent->winDialogue->texts.push_back({{"Pokonales mnie."},
                             {"Masz tu moja odznake Jazz Badge."},
                             {""}});
    opponentComponent->winDialogue->texts.push_back({{"Odblokowales dostep do nastepnej dzielnicy."},
                             {"Pokonaj kolejnego lidera w Electro Gymie."},
                             {""}});
    opponentComponent->lossDialogue->texts.push_back({{""},
                                                     {"Przegrales."},
                                                     {""}});

    auto gateDialogue = GameObject::Instantiate("GateDialogue", activeScene);
    gateDialogue->transform->SetLocalPosition(glm::vec3(-6, 0, -27));
    gateDialogue->transform->SetLocalScale(glm::vec3(0.6f));
    auto dialogAnimator = gateDialogue->AddComponent<Animator>();
    dialogAnimator->LoadAnimationModel("Crowd/BasicMan002/BasicMan002.dae");
    dialogAnimator->SetAnimation("CrowdAnimations/Idle3.dae");
    gateDialogue->AddComponent<BoxCollider>()->SetSize({2, 1, 2});
    gateDialogue->AddComponent<Dialogue>();
    gateDialogue->GetComponent<Dialogue>()->texts.push_back({{"Brama jest jeszcze zamknieta."},
                                                       {"Poczekaj na rozpoczecie konkursu"},
                                                       {""}});


    auto shopkeeper = GameObject::Instantiate("Shopkeeper", activeScene);
    shopkeeper->transform->SetLocalPosition(glm::vec3(1.5f, 0, -2));
    shopkeeper->AddComponent<Shopkeeper>();

    AIManager::GetInstance()->InitializeSpawner(10, 10, 100);

    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    auto pauseMenu = GloomEngine::GetInstance()->FindGameObjectWithName("Pause");
    if (pauseMenu)
        if (pauseMenu->GetComponent<PauseMenu>())
            shouldQuit = pauseMenu->GetComponent<PauseMenu>()->gameShouldExit;

    auto mainMenu = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu");
    if (mainMenu)
        if (mainMenu->GetComponent<MainMenu>())
            shouldQuit = mainMenu->GetComponent<MainMenu>()->gameShouldExit;


    return shouldQuit;
}
