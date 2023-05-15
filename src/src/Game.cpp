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
#include "Components/Animations/GameObjectAnimator.h"
#include "Components/Scripts/MainMenu.h"
#include "GameObjectsAndPrefabs/Prefabs/Player.h"
#include "GameObjectsAndPrefabs/Prefabs/Die.h"
#include "GameObjectsAndPrefabs/Prefabs/Shop.h"
#include "GameObjectsAndPrefabs/Prefabs/House.h"

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
    camera->cameraOffset = glm::vec3(0, 40, 40);

    // Set up cubemap
    // --------------
    auto sky = GameObject::Instantiate("CubeMap", activeScene);
    auto skyCubeMap = sky->AddComponent<CubeMap>();
    skyCubeMap->LoadTextures("skybox/");

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = Prefab::Instantiate<Player>();

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = Prefab::Instantiate<Die>("Ground");
    ground->transform->SetLocalPosition({0, -4, -10});
    ground->transform->SetLocalScale({40, 2, 40});
    ground->GetComponent<Renderer>()->textScale = glm::vec2(40, 40);

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<DirectionalLight>();
    sun->transform->SetLocalPosition({10, 20, 10});
    sun->transform->SetLocalRotation({-50, 70, 0});

    // Set up UI
    // ---------
    auto ui = GameObject::Instantiate("ui", activeScene)->AddComponent<Menu>();
    ui->AddImage("Reksio", 50, 0, "UI/piesek.png");
    ui->AddImage("Mruczek", 1742, 0, "UI/kotek.png");


    // Set up pause menu
    auto pause = GameObject::Instantiate("Pause", activeScene)->AddComponent<PauseMenu>();
    std::shared_ptr<GameObject> resumeButton = pause->AddButton("ResumeButton", 900, 600, "UI/buttonInactive.png", "UI/buttonActive.png", "Resume", 32);
    std::shared_ptr<GameObject> optionsButton = pause->AddButton("OptionsButton", 900, 500, "UI/buttonInactive.png", "UI/buttonActive.png", "Options", 32);
    std::shared_ptr<GameObject> exitToMainMenuButton = pause->AddButton("ExitToMainMenuButton", 900, 400, "UI/buttonInactive.png", "UI/buttonActive.png", "Main Menu", 32);
    std::shared_ptr<GameObject> exitButton = pause->AddButton("ExitButton", 900, 300, "UI/buttonInactive.png", "UI/buttonActive.png", "Exit", 32);
    std::shared_ptr<GameObject> pauseBackground = pause->AddImage("Background", 0, 0, "UI/pause.png");
    resumeButton->GetComponent<Button>()->previousButton = exitButton->GetComponent<Button>();
    resumeButton->GetComponent<Button>()->nextButton = optionsButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->previousButton = resumeButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->nextButton = exitToMainMenuButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->previousButton = optionsButton->GetComponent<Button>();
    exitToMainMenuButton->GetComponent<Button>()->nextButton = exitButton->GetComponent<Button>();
    exitButton->GetComponent<Button>()->previousButton = exitToMainMenuButton->GetComponent<Button>();
    exitButton->GetComponent<Button>()->nextButton = resumeButton->GetComponent<Button>();
    pause->GetParent()->DisableSelfAndChildren();

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
    std::shared_ptr<GameObject> shop = Prefab::Instantiate<Shop>();

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

    std::shared_ptr<GameObject> hydrant = GameObject::Instantiate("Hydrant", activeScene);
    hydrant->transform->SetLocalPosition({15, 0, -15});
    hydrant->transform->SetLocalRotation({0, -65, 0});
    hydrant->transform->SetLocalScale({0.5, 0.5, 0.5});
    hydrant->AddComponent<Renderer>()->LoadModel("texturedModels/hydrant.obj");


    // SCENE BUILDINGS
	std::vector<std::string> buildingPaths = {
			"jazz1", "jazz2", "jazz3", "jazz4", "kamienica1", "kamienica2", "kamienica3"
	};
	std::map<std::string, int> buildingSizes = {
			{"jazz1", 6},
			{"jazz2", 7},
			{"jazz3", 10},
			{"jazz4", 6},
			{"kamienica1", 6},
			{"kamienica2", 10},
			{"kamienica3", 6}
	};
	float currentXPos = -20;


	for(int i=0; i < buildingPaths.size(); i++) {
		currentXPos += buildingSizes[buildingPaths[i]]/2.0f;

		std::shared_ptr<GameObject> test = GameObject::Instantiate("TestHouse", activeScene);
		test->transform->SetLocalPosition({currentXPos, 0, -30});
		test->AddComponent<Renderer>()->LoadModel("Budynki/modele/"+buildingPaths[i]+".obj");
		test->AddComponent<BoxCollider>()->SetOffset({-6, -3, 0});
		test->GetComponent<BoxCollider>()->SetSize({6, 6, 3});

		currentXPos += buildingSizes[buildingPaths[i]]/2.0f;
	}

    std::shared_ptr<GameObject> test = GameObject::Instantiate("TestHouse", activeScene);
    test->transform->SetLocalPosition({0, 0, -30});
    test->transform->SetLocalScale({1, 1, 1});
    test->AddComponent<Renderer>()->LoadModel("Budynki/modele/jazz1.obj");
    test->AddComponent<BoxCollider>()->SetOffset({-6, -3, 0});
    test->GetComponent<BoxCollider>()->SetSize({6, 6, 3});

    std::shared_ptr<GameObject> test1 = GameObject::Instantiate("TestHouse", activeScene);
    test1->transform->SetLocalPosition({7, 0, -30});
    test1->transform->SetLocalScale({1, 1, 1});
    test1->AddComponent<Renderer>()->LoadModel("Budynki/modele/jazz2.obj");
    test1->AddComponent<BoxCollider>()->SetOffset({-6, -3, 0});
    test1->GetComponent<BoxCollider>()->SetSize({6, 6, 3.5});

    std::shared_ptr<GameObject> test2 = GameObject::Instantiate("TestHouse", activeScene);
    test2->transform->SetLocalPosition({-9, 0, -30});
    test2->transform->SetLocalScale({1, 1, 1});
    test2->AddComponent<Renderer>()->LoadModel("Budynki/modele/jazz3.obj");
    test2->AddComponent<BoxCollider>()->SetOffset({-6, -3, 0});
    test2->GetComponent<BoxCollider>()->SetSize({6, 6, 5.5});

    std::shared_ptr<GameObject> test3 = GameObject::Instantiate("TestHouse", activeScene);
    test3->transform->SetLocalPosition({-19, 0, -30});
    test3->transform->SetLocalScale({1, 1, 1});
    test3->AddComponent<Renderer>()->LoadModel("Budynki/modele/jazz3.obj");
    test3->AddComponent<BoxCollider>()->SetOffset({-6, -3, 0});
    test3->GetComponent<BoxCollider>()->SetSize({6, 6, 5.5});

    std::shared_ptr<GameObject> test4 = GameObject::Instantiate("TestHouse", activeScene);
    test4->transform->SetLocalPosition({16, 0, -30});
    test4->transform->SetLocalScale({1, 1, 1});
    test4->AddComponent<Renderer>()->LoadModel("Budynki/modele/jazz4.obj");
    test4->AddComponent<BoxCollider>()->SetOffset({-6, -3, 0});
    test4->GetComponent<BoxCollider>()->SetSize({6, 6, 3});


    Animator::LoadAnimation("Animacje/BasicChlop.dae");

	// Set up animated model
    for (int i = 0; i < 10; ++i) {
        std::shared_ptr<GameObject> animatedDood = GameObject::Instantiate("DOOD", SceneManager::GetInstance()->activeScene, Tags::DEFAULT);
        auto animatedDoodAnimator = animatedDood->AddComponent<Animator>();
        animatedDoodAnimator->LoadAnimationModel("Animacje/BasicChlop.dae");
        animatedDoodAnimator->SetAnimation("Animacje/BasicChlop.dae");
        animatedDood->transform->SetLocalPosition({-15 + i, 1, -10});
        animatedDood->transform->SetLocalRotation({0, 0, 0});
        animatedDood->transform->SetLocalScale({0.5, 0.5, 0.5});
    }

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("Pause"))
        if (GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>())
            shouldQuit = GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->gameShouldExit;

    if (GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu"))
        if (GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu")->GetComponent<MainMenu>())
            shouldQuit = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu")->GetComponent<MainMenu>()->gameShouldExit;


    return shouldQuit;
}
