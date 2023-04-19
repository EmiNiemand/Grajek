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

//    std::shared_ptr<GameObject> bench = GameObject::Instantiate("Bench", activeScene);
//    bench->transform->SetLocalPosition({0, 0, -20});
//    bench->transform->SetLocalRotation({0, -90, 0});
//    bench->transform->SetLocalScale({0.5, 0.5, 0.5});
//    bench->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
//    bench->AddComponent<BoxCollider>()->SetOffset({5, 1, -2.5});
//    bench->GetComponent<BoxCollider>()->SetSize({2, 2, 3});


//    std::shared_ptr<GameObject> pointLight = GameObject::Instantiate("pointLight", activeScene);
//    pointLight->AddComponent<PointLight>();
//    pointLight->transform->SetLocalPosition({0, 4, -10});
//    pointLight->GetComponent<PointLight>()->SetAmbient({0.2, 0.2, 0.2});
//    pointLight->GetComponent<PointLight>()->SetDiffuse({0.6, 0.6, 0.6});
//    pointLight->GetComponent<PointLight>()->SetSpecular({0.9, 0.9, 0.9});

    // Set up UI
    // ---------
    std::shared_ptr<GameObject> reksio = GameObject::Instantiate("Reksio", activeScene);
    reksio->AddComponent<Image>();
    reksio->GetComponent<Image>()->LoadTexture(50, 0, "UI/piesek.png");
    std::shared_ptr<GameObject> mruczek = GameObject::Instantiate("Mruczek", activeScene);
    mruczek->AddComponent<Image>();
    mruczek->GetComponent<Image>()->LoadTexture(1650, 0, "UI/kotek.png");

    // Set up pause menu
    std::shared_ptr<GameObject> pause = GameObject::Instantiate("Pause", activeScene);
    pause->AddComponent<PauseMenu>();
    std::shared_ptr<GameObject> optionsButton = GameObject::Instantiate("OptionsButton", pause);
    optionsButton->AddComponent<Button>();
    optionsButton->GetComponent<Button>()->LoadTexture(900, 500, "UI/buttonInactive.png", "UI/buttonActive.png");
    optionsButton->GetComponent<Button>()->LoadFont("Options", 925, 525, 40);
    std::shared_ptr<GameObject> exitToMainMenuButton = GameObject::Instantiate("ExitToMainMenu", pause);
    exitToMainMenuButton->AddComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->LoadTexture(900, 400, "UI/buttonInactive.png", "UI/buttonActive.png");
    exitToMainMenuButton->GetComponent<Button>()->LoadFont("Exit", 960, 425, 40);
    std::shared_ptr<GameObject> pauseBackground = GameObject::Instantiate("Background", pause);
    pauseBackground->AddComponent<Image>();
    pauseBackground->GetComponent<Image>()->LoadTexture(0, 0, "UI/pause.png");
    optionsButton->GetComponent<Button>()->previousButton = exitToMainMenuButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->nextButton = exitToMainMenuButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->previousButton = optionsButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->nextButton = optionsButton->GetComponent<Button>();
    pause->DisableSelfAndChildren();

    // Set up options menu
    auto options = GameObject::Instantiate("Options", activeScene);
    options->AddComponent<OptionsMenu>();
    auto backToPauseMenu = GameObject::Instantiate("BackToPauseMenu", options)->AddComponent<Button>();
    backToPauseMenu->LoadTexture(900, 500, "UI/buttonInactive.png", "UI/buttonActive.png");
    backToPauseMenu->LoadFont("Back", 960, 525, 40);
    auto decreaseVolume = GameObject::Instantiate("DecreaseVolume", options)->AddComponent<Button>();
    decreaseVolume->LoadTexture(1000, 780, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png");
    decreaseVolume->LoadFont("-", 1025, 795, 64);
    auto increaseVolume = GameObject::Instantiate("IncreaseVolume", options)->AddComponent<Button>();
    increaseVolume->LoadTexture(1300, 780, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png");
    increaseVolume->LoadFont("+", 1325, 795, 64);
    auto decreaseResolution = GameObject::Instantiate("DecreaseResolution", options)->AddComponent<Button>();
    decreaseResolution->LoadTexture(1000, 680, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png");
    decreaseResolution->LoadFont("<", 1025, 695, 64);
    auto increaseResolution = GameObject::Instantiate("IncreaseResolution", options)->AddComponent<Button>();
    increaseResolution->LoadTexture(1300, 680, "UI/buttonSmallInactive.png", "UI/buttonSmallActive.png");
    increaseResolution->LoadFont(">", 1325, 695, 64);
    auto musicVolume = GameObject::Instantiate("MusicVolume", options)->AddComponent<Text>();
    musicVolume->LoadFont("Music Volume", 500, 800, 42);
    auto musicVolumeValue = GameObject::Instantiate("MusicVolumeValue", options)->AddComponent<Text>();
    musicVolumeValue->LoadFont("10", 1150, 800, 42);
    auto windowResolution = GameObject::Instantiate("WindowResolution", options)->AddComponent<Text>();
    windowResolution->LoadFont("Window Resolution", 500, 700, 42);
    auto windowResolutionValue = GameObject::Instantiate("WindowResolutionValue", options)->AddComponent<Text>();
    windowResolutionValue->LoadFont("1440 x 810", 1075, 700, 42);
    std::shared_ptr<GameObject> optionsBackground = GameObject::Instantiate("OptionsBackground", options);
    optionsBackground->AddComponent<Image>();
    optionsBackground->GetComponent<Image>()->LoadTexture(0, 0, "UI/options.png");
    decreaseVolume->previousButton = backToPauseMenu;
    decreaseVolume->nextButton = increaseVolume;
    increaseVolume->previousButton = decreaseVolume;
    increaseVolume->nextButton = decreaseResolution;
    decreaseResolution->previousButton = increaseVolume;
    decreaseResolution->nextButton = increaseResolution;
    increaseResolution->previousButton = decreaseResolution;
    increaseResolution->nextButton = backToPauseMenu;
    backToPauseMenu->previousButton = increaseResolution;
    backToPauseMenu->nextButton = decreaseVolume;
    options->DisableSelfAndChildren();

    // Set up shop menu
    std::shared_ptr<GameObject> shop = GameObject::Instantiate("Shop", activeScene);
    shop->AddComponent<ShopMenu>();
    auto firstInstrument = GameObject::Instantiate("FirstInstrument", shop)->AddComponent<Button>();
    firstInstrument->LoadTexture(10, 0, "UI/Sklep/Perkusja.png", "UI/Sklep/PerkusjaZRamka.png");
    auto secondInstrument = GameObject::Instantiate("SecondInstrument", shop)->AddComponent<Button>();
    secondInstrument->LoadTexture(1425, 525, "UI/Sklep/Trabka.png", "UI/Sklep/TrabkaZRamka.png");
    auto thirdInstrument = GameObject::Instantiate("ThirdInstrument", shop)->AddComponent<Button>();
    thirdInstrument->LoadTexture(1525, 250, "UI/Sklep/LaunbhPad.png", "UI/Sklep/LaunbhPadZRamka.png");
    auto fourthInstrument = GameObject::Instantiate("FourthInstrument", shop)->AddComponent<Button>();
    fourthInstrument->LoadTexture(600, 700, "UI/Sklep/Gitara.png", "UI/Sklep/GitaraZRamka.png");
    auto shopBackground = GameObject::Instantiate("ShopBackground", shop)->AddComponent<Image>();
    shopBackground->LoadTexture(0, 0, "UI/Sklep/Sklep.png");
    firstInstrument->previousButton = thirdInstrument;
    firstInstrument->nextButton = fourthInstrument;
    secondInstrument->previousButton = fourthInstrument;
    secondInstrument->nextButton = thirdInstrument;
    thirdInstrument->previousButton = secondInstrument;
    thirdInstrument->nextButton = firstInstrument;
    fourthInstrument->previousButton = firstInstrument;
    fourthInstrument->nextButton = secondInstrument;
    shop->DisableSelfAndChildren();


    std::shared_ptr<GameObject> sphere = GameObject::Instantiate("Sphere", activeScene);
    sphere->transform->SetLocalPosition({-5, 2, 0});
    sphere->transform->SetLocalScale({2, 2, 2});
    sphere->AddComponent<Renderer>()->LoadModel("sphere/sphere.obj");
    std::shared_ptr<Renderer> rSphere = sphere->GetComponent<Renderer>();
    rSphere->material.refraction = 1.0f;

    std::shared_ptr<GameObject> lowPolyHouse1 = GameObject::Instantiate("House", activeScene);
    lowPolyHouse1->transform->SetLocalPosition({-15, 0, -20});
    lowPolyHouse1->transform->SetLocalRotation({0, -110, 0});
    lowPolyHouse1->transform->SetLocalScale({1.5, 1.5, 2});
    lowPolyHouse1->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
    lowPolyHouse1->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
    lowPolyHouse1->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});

    std::shared_ptr<GameObject> lowPolyHouse2 = GameObject::Instantiate("House", activeScene);
    lowPolyHouse2->transform->SetLocalPosition({-5, 0, -20});
    lowPolyHouse2->transform->SetLocalRotation({0, -110, 0});
    lowPolyHouse2->transform->SetLocalScale({1.5, 1.5, 2});
    lowPolyHouse2->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
    lowPolyHouse2->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
    lowPolyHouse2->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});

    std::shared_ptr<GameObject> lowPolyHouse3 = GameObject::Instantiate("House", activeScene);
    lowPolyHouse3->transform->SetLocalPosition({5, 0, -20});
    lowPolyHouse3->transform->SetLocalRotation({0, -110, 0});
    lowPolyHouse3->transform->SetLocalScale({1.5, 1.5, 2});
    lowPolyHouse3->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
    lowPolyHouse3->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
    lowPolyHouse3->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});

    std::shared_ptr<GameObject> lowPolyHouse = GameObject::Instantiate("House", activeScene);
    lowPolyHouse->transform->SetLocalPosition({15, 0, -20});
    lowPolyHouse->transform->SetLocalRotation({0, -110, 0});
    lowPolyHouse->transform->SetLocalScale({1.5, 1.5, 2});
    lowPolyHouse->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
    lowPolyHouse->GetComponent<Renderer>()->material.refraction = 1.0f;
    lowPolyHouse->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
    lowPolyHouse->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});

//    std::shared_ptr<GameObject> pub = GameObject::Instantiate("Pub", activeScene);
//    pub->transform->SetLocalPosition({-10, 0, -20});
//    pub->transform->SetLocalRotation({0, -60, 0});
//    pub->transform->SetLocalScale({1.5, 1.5, 1.5});
//    pub->AddComponent<Renderer>()->LoadModel("texturedModels/pub.obj");
//    pub->AddComponent<BoxCollider>()->SetOffset({0, 1.5, 0});
//    pub->GetComponent<BoxCollider>()->SetSize({3, 1.5, 3.5});

//    std::shared_ptr<GameObject> hydrant = GameObject::Instantiate("Hydrant", activeScene);
//    hydrant->transform->SetLocalPosition({15, 0, -15});
//    hydrant->transform->SetLocalRotation({0, -65, 0});
//    hydrant->transform->SetLocalScale({0.5, 0.5, 0.5});
//    hydrant->AddComponent<Renderer>()->LoadModel("texturedModels/hydrant.obj");

	// Set up animated model
//	std::shared_ptr<GameObject> animatedDood = Prefab::GetDancingDude();
//	animatedDood->transform->SetLocalPosition({-2, 0, -10});
//	animatedDood->transform->SetLocalScale({1.5, 1.5, 1.5});

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    shouldQuit = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->gameShouldExit;

    return shouldQuit;
}
