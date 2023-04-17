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
    auto listener = player->AddComponent<AudioListener>();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::GetCube("Ground");
    ground->transform->SetLocalPosition({0, -4, -10});
    ground->transform->SetLocalScale({20, 2, 20});

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({5, 10, 5});
    sun->transform->SetLocalRotation({-50, 70, 0});

    std::shared_ptr<GameObject> bench = GameObject::Instantiate("Bench", activeScene);
    bench->transform->SetLocalPosition({0, 0, -20});
    bench->transform->SetLocalRotation({0, -90, 0});
    bench->transform->SetLocalScale({0.5, 0.5, 0.5});
    bench->AddComponent<Renderer>()->LoadModel("texturedModels/lawka.obj");
    bench->AddComponent<BoxCollider>()->SetOffset({5, 1, -2.5});
    bench->GetComponent<BoxCollider>()->SetSize({1.25, 2, 3});


//    std::shared_ptr<GameObject> pointLight = GameObject::Instantiate("pointLight", activeScene);
//    pointLight->AddComponent<PointLight>();
//    pointLight->transform->SetLocalPosition({0, 4, -10});
//    pointLight->GetComponent<PointLight>()->SetAmbient({0.2, 0.2, 0.2});
//    pointLight->GetComponent<PointLight>()->SetDiffuse({0.6, 0.6, 0.6});
//    pointLight->GetComponent<PointLight>()->SetSpecular({0.9, 0.9, 0.9});

    // Set up UI
    // ---------
    // Texts at the end
    // Texts at the end
    std::shared_ptr<GameObject> reksio = GameObject::Instantiate("Reksio", activeScene);
    reksio->AddComponent<Image>();
    reksio->GetComponent<Image>()->LoadTexture(50, 0, "UI/piesek.png");
    std::shared_ptr<GameObject> mruczek = GameObject::Instantiate("Mruczek", activeScene);
    mruczek->AddComponent<Image>();
    mruczek->GetComponent<Image>()->LoadTexture(1650, 0, "UI/kotek.png");
    std::shared_ptr<GameObject> tekst = GameObject::Instantiate("Tekst", activeScene);
    tekst->AddComponent<Text>();
    tekst->GetComponent<Text>()->LoadFont("easter egg", 1725, 10);

    // Set up pause menu
    std::shared_ptr<GameObject> pause = GameObject::Instantiate("Pause", activeScene);
    pause->AddComponent<PauseMenu>();
    std::shared_ptr<GameObject> pauseBackground = GameObject::Instantiate("Background", pause);
    pauseBackground->AddComponent<Image>();
    pauseBackground->GetComponent<Image>()->LoadTexture(0, 0, "UI/pause.png");
    std::shared_ptr<GameObject> optionsButton = GameObject::Instantiate("OptionsButton", pause);
    optionsButton->AddComponent<Button>();
    optionsButton->GetComponent<Button>()->LoadTexture(900, 500, "UI/button.png", "UI/activeButton.png");
    optionsButton->GetComponent<Button>()->LoadFont("Options", 885, 520);
    std::shared_ptr<GameObject> exitToMainMenuButton = GameObject::Instantiate("ExitToMainMenu", pause);
    exitToMainMenuButton->AddComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->LoadTexture(900, 400, "UI/button.png", "UI/activeButton.png");
    exitToMainMenuButton->GetComponent<Button>()->LoadFont("Exit", 905, 420);
    optionsButton->GetComponent<Button>()->previousButton = exitToMainMenuButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->nextButton = exitToMainMenuButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->previousButton = optionsButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->nextButton = optionsButton->GetComponent<Button>();
    pause->DisableSelfAndChildren();

    // Set up options menu
    std::shared_ptr<GameObject> options = GameObject::Instantiate("Options", activeScene);
    options->AddComponent<OptionsMenu>();
    std::shared_ptr<GameObject> optionsBackground = GameObject::Instantiate("OptionsBackground", options);
    optionsBackground->AddComponent<Image>();
    optionsBackground->GetComponent<Image>()->LoadTexture(0, 0, "UI/options.png");
    std::shared_ptr<GameObject> backToPauseMenu = GameObject::Instantiate("BackToPauseMenu", options);
    backToPauseMenu->AddComponent<Button>();
    backToPauseMenu->GetComponent<Button>()->LoadTexture(900, 500, "UI/button.png", "UI/activeButton.png");
    backToPauseMenu->GetComponent<Button>()->LoadFont("Back", 905, 520);
    std::shared_ptr<GameObject> decreaseVolume = GameObject::Instantiate("DecreaseVolume", options);
    decreaseVolume->AddComponent<Button>();
    decreaseVolume->GetComponent<Button>()->LoadTexture(1000, 800, "UI/button.png", "UI/activeButton.png");
    decreaseVolume->GetComponent<Button>()->LoadFont("-", 1010, 810, 64);
    std::shared_ptr<GameObject> increaseVolume = GameObject::Instantiate("IncreaseVolume", options);
    increaseVolume->AddComponent<Button>();
    increaseVolume->GetComponent<Button>()->LoadTexture(1100, 800, "UI/button.png", "UI/activeButton.png");
    increaseVolume->GetComponent<Button>()->LoadFont("+", 1110, 810, 64);
    std::shared_ptr<GameObject> decreaseResolution = GameObject::Instantiate("DecreaseResolution", options);
    decreaseResolution->AddComponent<Button>();
    decreaseResolution->GetComponent<Button>()->LoadTexture(1000, 700, "UI/button.png", "UI/activeButton.png");
    decreaseResolution->GetComponent<Button>()->LoadFont("<", 1010, 700, 64);
    std::shared_ptr<GameObject> increaseResolution = GameObject::Instantiate("IncreaseResolution", options);
    increaseResolution->AddComponent<Button>();
    increaseResolution->GetComponent<Button>()->LoadTexture(1100, 700, "UI/button.png", "UI/activeButton.png");
    increaseResolution->GetComponent<Button>()->LoadFont(">", 1110, 700, 64);
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
    std::shared_ptr<GameObject> shopBackground = GameObject::Instantiate("ShopBackground", shop);
    shopBackground->AddComponent<Image>();
    shopBackground->GetComponent<Image>()->LoadTexture(0, 0, "UI/Sklep/Sklep.png");
    std::shared_ptr<GameObject> firstInstrument = GameObject::Instantiate("FirstInstrument", shop);
    firstInstrument->AddComponent<Button>();
    firstInstrument->GetComponent<Button>()->LoadTexture(10, 0, "UI/Sklep/PerkusjaZRamka.png", "UI/Sklep/PerkusjaZRamka.png");
    std::shared_ptr<GameObject> secondInstrument = GameObject::Instantiate("SecondInstrument", shop);
    secondInstrument->AddComponent<Button>();
    secondInstrument->GetComponent<Button>()->LoadTexture(1425, 525, "UI/Sklep/TrabkaZRamka.png", "UI/Sklep/TrabkaZRamka.png");
    std::shared_ptr<GameObject> thirdInstrument = GameObject::Instantiate("ThirdInstrument", shop);
    thirdInstrument->AddComponent<Button>();
    thirdInstrument->GetComponent<Button>()->LoadTexture(1525, 250, "UI/Sklep/LaunbhPadZRamka.png", "UI/Sklep/LaunbhPadZRamka.png");
    std::shared_ptr<GameObject> fourthInstrument = GameObject::Instantiate("FourthInstrument", shop);
    fourthInstrument->AddComponent<Button>();
    fourthInstrument->GetComponent<Button>()->LoadTexture(600, 700, "UI/Sklep/GitaraZRamka.png", "UI/Sklep/GitaraZRamka.png");
    firstInstrument->GetComponent<Button>()->previousButton = fourthInstrument->GetComponent<Button>();
    firstInstrument->GetComponent<Button>()->nextButton = secondInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->previousButton = firstInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->nextButton = thirdInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->previousButton = secondInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->nextButton = fourthInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->previousButton = thirdInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->nextButton = firstInstrument->GetComponent<Button>();
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
    lowPolyHouse->GetComponent<BoxCollider>()->SetSize({3, 2, 1.5});
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
