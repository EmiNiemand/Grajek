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
#include "GameObjectsAndPrefabs/Prefabs/House.h"
#include "GameObjectsAndPrefabs/Prefabs/SavePoint.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/Scripts/Menus/Shopkeeper.h"
#include "EngineManagers/AIManager.h"
#include "GameObjectsAndPrefabs/Prefabs/OpponentPrefab.h"
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
    Animator::LoadAnimation("AnimsNew/Walk.dae");
    Animator::LoadAnimation("AnimsNew/Happy.dae");
    Animator::LoadAnimation("AnimsNew/Angry.dae");
    Animator::LoadAnimation("AnimsNew/Idle1.dae");
    Animator::LoadAnimation("AnimsNew/Idle3.dae");
    Animator::LoadAnimation("MainHero/MainHeroIdle.dae");
    Animator::LoadAnimation("MainHero/MainHeroRun.dae");
    Animator::LoadAnimation("MainHero/MainHeroTrumpet.dae");

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = Prefab::Instantiate<Player>();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::Instantiate<Die>("Ground");
    ground->transform->SetLocalPosition({0, -4, 0});
    ground->transform->SetLocalScale({30, 2, 30});
    ground->GetComponent<Renderer>()->textScale = glm::vec2(30, 30);

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({20, 40, 20});
    sun->transform->SetLocalRotation({-50, 70, 0});

    std::shared_ptr<GameObject> bench = GameObject::Instantiate("Bench", activeScene);
    bench->transform->SetLocalPosition({0, 0, -7});
    bench->transform->SetLocalRotation({0, 180, 0});
    bench->transform->SetLocalScale({0.5, 0.5, 0.5});
    bench->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
    bench->AddComponent<BoxCollider>()->SetOffset({4.5, 1, -2.5});
    bench->GetComponent<BoxCollider>()->SetSize({1.5, 2, 3});
    auto portalRadioSound = bench->AddComponent<AudioSource>();
    portalRadioSound->LoadAudioData("res/sounds/positional/portal_radio.wav", AudioType::Positional);
    portalRadioSound->SetPositionOffset({1.3f, 0.0f, 3.0f});
    portalRadioSound->SetDistanceMode(AudioDistanceMode::Continuous);
    portalRadioSound->SetMaxDistance(20.0f);
    portalRadioSound->SetCone({0.0f, 0.0f, 1.0f}, {110.0f, 200.0f});
    portalRadioSound->IsLooping(true);
    portalRadioSound->PlaySound();

    std::shared_ptr<GameObject> bench2 = GameObject::Instantiate("Bench", activeScene);
    bench2->transform->SetLocalPosition({10, 0, 0});
    bench2->transform->SetLocalRotation({0, 180, 0});
    bench2->transform->SetLocalScale({0.5, 0.5, 0.5});
    bench2->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
    bench2->AddComponent<BoxCollider>()->SetOffset({4.5, 1, -2.5});
    bench2->GetComponent<BoxCollider>()->SetSize({1.5, 2, 3});

    std::shared_ptr<GameObject> hydrant = GameObject::Instantiate("Hydrant", activeScene);
    hydrant->transform->SetLocalPosition({16, 0, 15});
    hydrant->transform->SetLocalRotation({0, 130, 0});
    hydrant->transform->SetLocalScale({0.5, 0.5, 0.5});
    hydrant->AddComponent<Renderer>()->LoadModel("texturedModels/hydrant.obj");

    auto savePoint1 = Prefab::Instantiate<SavePoint>();
    savePoint1->transform->SetLocalPosition({-15, 0, 10});
    savePoint1->transform->SetLocalScale({2.0, 2.0, 2.0});

    auto opponent = Prefab::Instantiate<OpponentPrefab>();
    opponent->transform->SetLocalPosition(glm::vec3(12, 0, -10));
    // 2      *   *
    // 1    *   *
    // 0  *
    opponent->children.begin()->second->AddComponent<Opponent>()->Setup(Instrument::GetInstrument(InstrumentName::Drums),
                                              {{0, 0.5}, {1, 0.5}, {2, 0.5}, {1, 0.5}, {2, 0.5}}, 80.0f);
    opponent->children.begin()->second->GetComponent<Opponent>()->dialogue->texts.push_back({{"Pokonales mnie."},
                             {"Masz tu moja odznake Jazz Badge."},
                             {""}});
    opponent->children.begin()->second->GetComponent<Opponent>()->dialogue->texts.push_back({{"Odblokowales dostep do nastepnej dzielnicy."},
                             {"Pokonaj kolejnego lidera w Electro Gymie."},
                             {""}});

    auto dialog = GameObject::Instantiate("Dialog", activeScene);
    dialog->transform->SetLocalPosition(glm::vec3(17, 0, 2));
    dialog->AddComponent<Renderer>()->LoadModel("texturedModels/przeciwnik.obj");
    dialog->AddComponent<BoxCollider>()->SetSize({2, 1, 2});
    dialog->AddComponent<Dialogue>();
    dialog->GetComponent<Dialogue>()->texts.push_back({{""},
                                                       {"Tak to ja."},
                                                       {""}});
    dialog->GetComponent<Dialogue>()->texts.push_back({{""},
                                                       {"Walcz ze mna."},
                                                       {""}});

    camera->SetTarget(nullptr);

    AIManager::GetInstance()->InitializeSpawner(1, 1, 100);
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
