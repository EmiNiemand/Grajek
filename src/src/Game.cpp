#include "Game.h"
#include "GloomEngine.h"
#include "EngineManagers/SceneManager.h"
#include "EngineManagers/RendererManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/CubeMap.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/PlayerManager.h"
#include "Components/Scripts/PlayerMovement.h"
#include "Components/Scripts/PauseMenu.h"
#include "Components/Scripts/OptionsMenu.h"
#include "Components/Renderers/Animator.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Scripts/ShopMenu.h"
#include "Components/Animations/GameObjectAnimator.h"

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
    camera->cameraOffset = glm::vec3(0, 20, 20);

    // Set up cubemap
    // --------------
    auto skyCubeMap = GameObject::Instantiate("CubeMap", activeScene)->AddComponent<CubeMap>();
    skyCubeMap->LoadTextures("skybox/");

	// Set gamma
	// ---------
	RendererManager::GetInstance()->SetGamma(1.0f);

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = Prefab::GetPlayer();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::GetCube("Ground");
    ground->transform->SetLocalPosition({0, -4, -10});
    ground->transform->SetLocalScale({40, 2, 40});
    ground->GetComponent<Renderer>()->textScale = glm::vec2(40, 40);

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({10, 20, 10});
    sun->transform->SetLocalRotation({-50, 70, 0});

//    std::shared_ptr<GameObject> pointLight = GameObject::Instantiate("pointLight", activeScene);
//    pointLight->AddComponent<PointLight>();
//    pointLight->transform->SetLocalPosition({0, 4, -10});
//    pointLight->GetComponent<PointLight>()->SetAmbient({0.2, 0.2, 0.2});
//    pointLight->GetComponent<PointLight>()->SetDiffuse({0.6, 0.6, 0.6});
//    pointLight->GetComponent<PointLight>()->SetSpecular({0.9, 0.9, 0.9});

    // Set up UI
    // ---------
    auto ui = GameObject::Instantiate("ui", activeScene)->AddComponent<Menu>();
    ui->AddImage("Reksio", 50, 0, "UI/piesek.png");
    ui->AddImage("Mruczek", 1450, 0, "UI/kotek.png");


    // Set up pause menu
    std::shared_ptr<GameObject> pause = GameObject::Instantiate("Pause", activeScene);
    pause->AddComponent<PauseMenu>();
    std::shared_ptr<GameObject> resumeButton = pause->GetComponent<PauseMenu>()->Menu::AddButton("ResumeButton", 900, 600, "UI/buttonInactive.png", "UI/buttonActive.png", "Resume", 32);
    std::shared_ptr<GameObject> optionsButton = pause->GetComponent<PauseMenu>()->Menu::AddButton("OptionsButton", 900, 500, "UI/buttonInactive.png", "UI/buttonActive.png", "Options", 32);
    std::shared_ptr<GameObject> exitToMainMenuButton = pause->GetComponent<PauseMenu>()->Menu::AddButton("ExitToMainMenuButton", 900, 400, "UI/buttonInactive.png", "UI/buttonActive.png", "Exit to main menu", 32);
    std::shared_ptr<GameObject> exitButton = pause->GetComponent<PauseMenu>()->Menu::AddButton("ExitButton", 900, 300, "UI/buttonInactive.png", "UI/buttonActive.png", "Exit", 32);
    std::shared_ptr<GameObject> pauseBackground = pause->GetComponent<PauseMenu>()->Menu::AddImage("Background", 0, 0, "UI/pause.png");
    resumeButton->GetComponent<Button>()->previousButton = exitButton->GetComponent<Button>();
    resumeButton->GetComponent<Button>()->nextButton = optionsButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->previousButton = resumeButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->nextButton = exitToMainMenuButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->previousButton = optionsButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->nextButton = exitButton->GetComponent<Button>();
    exitButton->GetComponent<Button>()->previousButton = exitToMainMenuButton->GetComponent<Button>();
    exitButton->GetComponent<Button>()->nextButton = resumeButton->GetComponent<Button>();
    pause->DisableSelfAndChildren();

    // Set up options menu
    auto options = GameObject::Instantiate("Options", activeScene)->AddComponent<OptionsMenu>();
    std::shared_ptr<GameObject> backToPauseMenu = options->Menu::AddButton("BackToPauseMenu", 380, 870, "UI/Opcje/Guzik.png", "UI/Opcje/GuzikZRamka.png");
    std::shared_ptr<GameObject> musicVolume = options->Menu::AddButton("MusicVolume", 538, 600, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
    std::shared_ptr<GameObject> windowResolution = options->Menu::AddButton("WindowResolution", 790, 395, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
    std::shared_ptr<GameObject> windowFullScreen = options->Menu::AddButton("WindowFullScreen", 1041, 175, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
    std::shared_ptr<GameObject> shadowResolution = options->Menu::AddButton("ShadowResolution", 1296, 600, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
    std::shared_ptr<GameObject> previousValue = options->Menu::AddButton("PreviousValue", 767, 882, "UI/Opcje/Tekst.png", "UI/Opcje/Suwak.png", "", 30);
    std::shared_ptr<GameObject> currentValue = options->Menu::AddButton("CurrentValue", 767, 845, "UI/Opcje/Tekst.png", "UI/Opcje/Suwak.png", "", 42);
    std::shared_ptr<GameObject> nextValue = options->Menu::AddButton("NextValue", 767, 808, "UI/Opcje/Tekst.png", "UI/Opcje/Suwak.png", "", 30);
    std::shared_ptr<GameObject> optionsBackground = options->Menu::AddImage("OptionsBackground", 285, 40, "UI/Opcje/Ustawienia.png");
    backToPauseMenu->GetComponent<Button>()->previousButton = shadowResolution->GetComponent<Button>();
    backToPauseMenu->GetComponent<Button>()->nextButton = musicVolume->GetComponent<Button>();
    musicVolume->GetComponent<Button>()->previousButton = backToPauseMenu->GetComponent<Button>();
    musicVolume->GetComponent<Button>()->nextButton = windowResolution->GetComponent<Button>();
    windowResolution->GetComponent<Button>()->previousButton = musicVolume->GetComponent<Button>();
    windowResolution->GetComponent<Button>()->nextButton = windowFullScreen->GetComponent<Button>();
    windowFullScreen->GetComponent<Button>()->previousButton = windowResolution->GetComponent<Button>();
    windowFullScreen->GetComponent<Button>()->nextButton = shadowResolution->GetComponent<Button>();
    shadowResolution->GetComponent<Button>()->previousButton = windowFullScreen->GetComponent<Button>();
    shadowResolution->GetComponent<Button>()->nextButton = backToPauseMenu->GetComponent<Button>();
    options->GetParent()->DisableSelfAndChildren();

    // Set up shop menu
    std::shared_ptr<GameObject> shop = Prefab::GetShop();


//    std::shared_ptr<GameObject> lowPolyHouse = GameObject::Instantiate("House", activeScene);
//    lowPolyHouse->transform->SetLocalPosition({15, 0, -20});
//    lowPolyHouse->transform->SetLocalRotation({0, -110, 0});
//    lowPolyHouse->transform->SetLocalScale({1.5, 1.5, 2});
//    lowPolyHouse->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
//    lowPolyHouse->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
//    lowPolyHouse->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});
//    auto portalRadioSound = lowPolyHouse->AddComponent<AudioSource>();
//    portalRadioSound->LoadAudioData("res/sounds/positional/portal_radio.wav", AudioType::Positional);
//    portalRadioSound->SetPositionOffset({0.0f, 0.0f, 4.5f});
//    portalRadioSound->SetDistanceMode(AudioDistanceMode::Continuous);
//    portalRadioSound->SetMaxDistance(20.0f);
//    portalRadioSound->SetCone({0.0f, 0.0f, 1.0f}, {110.0f, 200.0f});
//    portalRadioSound->IsLooping(true);
//    portalRadioSound->PlaySound();

    std::shared_ptr<GameObject> bench = GameObject::Instantiate("Bench", activeScene);
    bench->transform->SetLocalPosition({0, 0, -10});
    bench->transform->SetLocalRotation({0, -90, 0});
    bench->transform->SetLocalScale({0.5, 0.5, 0.5});
    bench->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
    bench->AddComponent<BoxCollider>()->SetOffset({5, 1, -2.5});
    bench->GetComponent<BoxCollider>()->SetSize({2, 2, 3});
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
    bench2->AddComponent<BoxCollider>()->SetOffset({5, 1, -2.5});
    bench2->GetComponent<BoxCollider>()->SetSize({2, 2, 3});

//    GameObject::Instantiate("BenchAnimator")->AddComponent<GameObjectAnimator>()->Setup(
//            bench->transform, {
//                    {AnimatedProperty::Position, glm::vec3(0, 10, 0), 0.5f},
//                    {AnimatedProperty::Position, glm::vec3(0, -10, 0), 0.5f},
//            }, true);
//
//    GameObject::Instantiate("BenchAnimator")->AddComponent<GameObjectAnimator>()->Setup(
//            bench->transform, {
//                    {AnimatedProperty::Scale, glm::vec3(2)},
//                    {AnimatedProperty::Scale, glm::vec3(0.5f)},
//            }, true);
//
//    GameObject::Instantiate("BenchAnimator")->AddComponent<GameObjectAnimator>()->Setup(
//            bench2->transform, {
//                    {AnimatedProperty::Position, glm::vec3(0, 10, 0)},
//                    {AnimatedProperty::Position, glm::vec3(0, -10, 0)},
//            }, true);

    int maxHouses = 7;
    int houseOffset = 7;
    for (int i = 0; i < maxHouses; ++i) {
        if(i == ceil(maxHouses/2.0f) - 1) continue;

        float houseDistance = houseOffset*maxHouses/2.0f + 5.0f;
        float housePlacement = houseOffset * (i - maxHouses/2.0f + 1/2.0f);

        std::shared_ptr<GameObject> serialHouse = GameObject::Instantiate("House", activeScene);
        serialHouse->transform->SetLocalPosition({housePlacement, 0, -houseDistance});
        serialHouse->transform->SetLocalRotation({0, -90, 0});
        serialHouse->transform->SetLocalScale({1.5, 1.5, 2});
        serialHouse->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
        serialHouse->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
        serialHouse->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});

        std::shared_ptr<GameObject> serialHouseLeft = GameObject::Instantiate("House", activeScene);
        serialHouseLeft->transform->SetLocalPosition({-houseDistance, 0,  housePlacement});
        serialHouseLeft->transform->SetLocalRotation({0, 0, 0});
        serialHouseLeft->transform->SetLocalScale({1.5, 1.5, 2});
        serialHouseLeft->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
        serialHouseLeft->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
        serialHouseLeft->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});

        std::shared_ptr<GameObject> serialHouseRight = GameObject::Instantiate("House", activeScene);
        serialHouseRight->transform->SetLocalPosition({houseDistance, 0,  housePlacement});
        serialHouseRight->transform->SetLocalRotation({0, 180, 0});
        serialHouseRight->transform->SetLocalScale({1.5, 1.5, 2});
        serialHouseRight->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
        serialHouseRight->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
        serialHouseRight->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});
    }

    std::shared_ptr<GameObject> hydrant = GameObject::Instantiate("Hydrant", activeScene);
    hydrant->transform->SetLocalPosition({15, 0, -15});
    hydrant->transform->SetLocalRotation({0, -65, 0});
    hydrant->transform->SetLocalScale({0.5, 0.5, 0.5});
    hydrant->AddComponent<Renderer>()->LoadModel("texturedModels/hydrant.obj");

	// Set up animated model
//	std::shared_ptr<GameObject> animatedDood = Prefab::GetDancingDude();
//	animatedDood->transform->SetLocalPosition({-2, 0, -10});
//	animatedDood->transform->SetLocalScale({1.5, 1.5, 1.5});

//    std::shared_ptr<GameObject> sphere = GameObject::Instantiate("Sphere", activeScene);
//    sphere->transform->SetLocalPosition({-5, 2, 0});
//    sphere->transform->SetLocalScale({2, 2, 2});
//    sphere->AddComponent<Renderer>()->LoadModel("sphere/sphere.obj");
//    std::shared_ptr<Renderer> rSphere = sphere->GetComponent<Renderer>();
//    rSphere->material.refraction = 1.0f;

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    shouldQuit = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->gameShouldExit;


    return shouldQuit;
}
