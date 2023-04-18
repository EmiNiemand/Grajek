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

Game::Game() {
    activeCamera = Camera::activeCamera;
    activeScene = SceneManager::GetInstance()->activeScene;
}

Game::~Game() {}

void Game::InitializeGame() {
    // Set up camera
    // -------------
    std::shared_ptr<Camera> camera = activeCamera->AddComponent<Camera>();
    camera->cameraOffset = glm::vec3(0, 20, 20);

    // Set up cubemap
    // --------------
    auto sky = GameObject::Instantiate("CubeMap", activeScene);
    auto skyCubeMap = sky->AddComponent<CubeMap>();
    skyCubeMap->LoadTextures("skybox/");

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = Prefab::GetPlayer();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::GetCube("Ground");
    ground->transform->SetLocalPosition({0, -4, -10});
    ground->transform->SetLocalScale({40, 2, 40});

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({10, 20, 10});
    sun->transform->SetLocalRotation({-50, 70, 0});

    std::shared_ptr<GameObject> bench = GameObject::Instantiate("Bench", activeScene);
    bench->transform->SetLocalPosition({0, 0, -20});
    bench->transform->SetLocalRotation({0, -90, 0});
    bench->transform->SetLocalScale({0.5, 0.5, 0.5});
    bench->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
    bench->AddComponent<BoxCollider>()->SetOffset({5, 1, -2.5});
    bench->GetComponent<BoxCollider>()->SetSize({2, 2, 3});


//    std::shared_ptr<GameObject> pointLight = GameObject::Instantiate("pointLight", activeScene);
//    pointLight->AddComponent<PointLight>();
//    pointLight->transform->SetLocalPosition({0, 4, -10});
//    pointLight->GetComponent<PointLight>()->SetAmbient({0.2, 0.2, 0.2});
//    pointLight->GetComponent<PointLight>()->SetDiffuse({0.6, 0.6, 0.6});
//    pointLight->GetComponent<PointLight>()->SetSpecular({0.9, 0.9, 0.9});

    // Set up UI
    // ---------
    std::shared_ptr<GameObject> ui = GameObject::Instantiate("ui", activeScene);
    ui->AddComponent<Menu>();
    ui->GetComponent<Menu>()->AddImage("Reksio", 50, 0, "UI/piesek.png");
    ui->GetComponent<Menu>()->AddImage("Mruczek", 1650, 0, "UI/kotek.png");

    // Set up pause menu
    std::shared_ptr<GameObject> pause = GameObject::Instantiate("Pause", activeScene);
    pause->AddComponent<PauseMenu>();
    std::shared_ptr<GameObject> optionsButton = pause->GetComponent<PauseMenu>()->Menu::AddButton("OptionsButton", 900, 500, "UI/buttonInactive.png", "UI/buttonActive.png", "Options", 32);
    std::shared_ptr<GameObject> exitToMainMenuButton = pause->GetComponent<PauseMenu>()->Menu::AddButton("ExitToMainMenu", 900, 400, "UI/buttonInactive.png", "UI/buttonActive.png", "Exit", 32);
    std::shared_ptr<GameObject> pauseBackground = pause->GetComponent<PauseMenu>()->Menu::AddImage("Background", 0, 0, "UI/pause.png");
    optionsButton->GetComponent<Button>()->previousButton = exitToMainMenuButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->nextButton = exitToMainMenuButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->previousButton = optionsButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->nextButton = optionsButton->GetComponent<Button>();
    pause->DisableSelfAndChildren();

    // Set up options menu
    std::shared_ptr<GameObject> options = GameObject::Instantiate("Options", activeScene);
    options->AddComponent<OptionsMenu>();
    std::shared_ptr<GameObject> backToPauseMenu = options->GetComponent<OptionsMenu>()->Menu::AddButton("BackToPauseMenu", 900, 500, "UI/buttonInactive.png", "UI/buttonActive.png", "Back", 32);
    std::shared_ptr<GameObject> decreaseVolume = options->GetComponent<OptionsMenu>()->Menu::AddButton("DecreaseVolume", 1000, 800, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png", "-", 64);
    std::shared_ptr<GameObject> increaseVolume = options->GetComponent<OptionsMenu>()->Menu::AddButton("IncreaseVolume", 1300, 800, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png", "+", 64);
    std::shared_ptr<GameObject> decreaseResolution = options->GetComponent<OptionsMenu>()->Menu::AddButton("DecreaseResolution", 1000, 700, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png", "<", 64);
    std::shared_ptr<GameObject> increaseResolution = options->GetComponent<OptionsMenu>()->Menu::AddButton("IncreaseResolution", 1300, 700, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png", ">", 64);
    std::shared_ptr<GameObject> musicVolume = options->GetComponent<OptionsMenu>()->Menu::AddText("MusicVolume", "Music Volume", 500, 800, 42);
    std::shared_ptr<GameObject> musicVolumeValue = options->GetComponent<OptionsMenu>()->Menu::AddText("MusicVolumeValue", "10", 1150, 800, 42);
    std::shared_ptr<GameObject> windowResolution = options->GetComponent<OptionsMenu>()->Menu::AddText("WindowResolution", "Window Resolution", 500, 700, 42);
    std::shared_ptr<GameObject> windowResolutionValue = options->GetComponent<OptionsMenu>()->Menu::AddText("WindowResolutionValue", "1440 x 810", 1075, 700, 42);
    std::shared_ptr<GameObject> optionsBackground = options->GetComponent<OptionsMenu>()->Menu::AddImage("OptionsBackground", 0, 0, "UI/options.png");
    decreaseVolume->GetComponent<Button>()->previousButton = backToPauseMenu->GetComponent<Button>();
    decreaseVolume->GetComponent<Button>()->nextButton = increaseVolume->GetComponent<Button>();
    increaseVolume->GetComponent<Button>()->previousButton = decreaseVolume->GetComponent<Button>();
    increaseVolume->GetComponent<Button>()->nextButton = decreaseResolution->GetComponent<Button>();
    decreaseResolution->GetComponent<Button>()->previousButton = increaseVolume->GetComponent<Button>();
    decreaseResolution->GetComponent<Button>()->nextButton = increaseResolution->GetComponent<Button>();
    increaseResolution->GetComponent<Button>()->previousButton = decreaseResolution->GetComponent<Button>();
    increaseResolution->GetComponent<Button>()->nextButton = backToPauseMenu->GetComponent<Button>();
    backToPauseMenu->GetComponent<Button>()->previousButton = increaseResolution->GetComponent<Button>();
    backToPauseMenu->GetComponent<Button>()->nextButton = decreaseVolume->GetComponent<Button>();
    options->DisableSelfAndChildren();

    // Set up shop menu
    std::shared_ptr<GameObject> shop = GameObject::Instantiate("Shop", activeScene);
    shop->AddComponent<ShopMenu>();
    std::shared_ptr<GameObject> firstInstrument = shop->GetComponent<ShopMenu>()->Menu::AddButton("FirstInstrument", 10, 0, "UI/Sklep/Perkusja.png", "UI/Sklep/PerkusjaZRamka.png");
    std::shared_ptr<GameObject> secondInstrument = shop->GetComponent<ShopMenu>()->Menu::AddButton("SecondInstrument", 1425, 525, "UI/Sklep/Trabka.png", "UI/Sklep/TrabkaZRamka.png");
    std::shared_ptr<GameObject> thirdInstrument = shop->GetComponent<ShopMenu>()->Menu::AddButton("ThirdInstrument", 1525, 250, "UI/Sklep/LaunbhPad.png", "UI/Sklep/LaunbhPadZRamka.png");
    std::shared_ptr<GameObject> fourthInstrument = shop->GetComponent<ShopMenu>()->Menu::AddButton("FourthInstrument", 600, 700, "UI/Sklep/Gitara.png", "UI/Sklep/GitaraZRamka.png");
    std::shared_ptr<GameObject> shopBackground = shop->GetComponent<ShopMenu>()->Menu::AddImage("ShopBackground", 0, 0, "UI/Sklep/Sklep.png");
    firstInstrument->GetComponent<Button>()->previousButton = thirdInstrument->GetComponent<Button>();
    firstInstrument->GetComponent<Button>()->nextButton = fourthInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->previousButton = fourthInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->nextButton = thirdInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->previousButton = secondInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->nextButton = firstInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->previousButton = firstInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->nextButton = secondInstrument->GetComponent<Button>();
    shop->DisableSelfAndChildren();


    std::shared_ptr<GameObject> sphere = GameObject::Instantiate("Sphere", activeScene);
    sphere->transform->SetLocalPosition({-5, 2, 0});
    sphere->transform->SetLocalScale({2, 2, 2});
    sphere->AddComponent<Renderer>()->LoadModel("sphere/sphere.obj");
    std::shared_ptr<Renderer> rSphere = sphere->GetComponent<Renderer>();
    rSphere->material.refraction = 1.0f;

    std::shared_ptr<GameObject> lowPolyHouse = GameObject::Instantiate("House", activeScene);
    lowPolyHouse->transform->SetLocalPosition({15, 0, -20});
    lowPolyHouse->transform->SetLocalRotation({0, -110, 0});
    lowPolyHouse->transform->SetLocalScale({1.5, 1.5, 2});
    lowPolyHouse->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
    lowPolyHouse->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
    lowPolyHouse->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});
    auto portalRadioSound = lowPolyHouse->AddComponent<AudioSource>();
    portalRadioSound->LoadAudioData("res/sounds/portal_radio.wav", AudioType::Sound);
    portalRadioSound->SetPositionOffset({0.0f, 0.0f, 2.5f});
    portalRadioSound->SetDistanceMode(AudioDistanceMode::Continuous);
    portalRadioSound->SetMaxDistance(20.0f);
    portalRadioSound->SetCone({0.0f, 0.0f, 1.0f}, {110.0f, 200.0f});
    portalRadioSound->IsLooping(true);
    portalRadioSound->PlaySound();


    std::shared_ptr<GameObject> pub = GameObject::Instantiate("Pub", activeScene);
    pub->transform->SetLocalPosition({-10, 0, -20});
    pub->transform->SetLocalRotation({0, -60, 0});
    pub->transform->SetLocalScale({1.5, 1.5, 1.5});
    pub->AddComponent<Renderer>()->LoadModel("texturedModels/pub.obj");
    pub->AddComponent<BoxCollider>()->SetOffset({0, 1.5, 0});
    pub->GetComponent<BoxCollider>()->SetSize({3, 1.5, 3.5});

    std::shared_ptr<GameObject> hydrant = GameObject::Instantiate("Hydrant", activeScene);
    hydrant->transform->SetLocalPosition({15, 0, -15});
    hydrant->transform->SetLocalRotation({0, -65, 0});
    hydrant->transform->SetLocalScale({0.5, 0.5, 0.5});
    hydrant->AddComponent<Renderer>()->LoadModel("texturedModels/hydrant.obj");

	// Set up animated model
	std::shared_ptr<GameObject> animatedDood = Prefab::GetDancingDude();
	animatedDood->transform->SetLocalPosition({-2, 0, -10});
	animatedDood->transform->SetLocalScale({1.5, 1.5, 1.5});

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    return false;
}
