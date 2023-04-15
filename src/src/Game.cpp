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
    std::shared_ptr<GameObject> player2 = Prefab::GetPlayer();

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

    for (int i = 0; i < 2; i++) {
        std::shared_ptr<GameObject> sceneProp = Prefab::GetCube();
        sceneProp->transform->SetLocalPosition({i * std::cos(i) * 10, 0, -20 + i * std::sin(i)});
        sceneProp->transform->SetLocalRotation({0, cos(i) * 90, 0});
    }


//    std::shared_ptr<GameObject> pointLight = GameObject::Instantiate("pointLight", activeScene);
//    pointLight->AddComponent<PointLight>();
//    pointLight->transform->SetLocalPosition({0, 4, -10});
//    pointLight->GetComponent<PointLight>()->SetAmbient({0.2, 0.2, 0.2});
//    pointLight->GetComponent<PointLight>()->SetDiffuse({0.6, 0.6, 0.6});
//    pointLight->GetComponent<PointLight>()->SetSpecular({0.9, 0.9, 0.9});

    // Set up UI
    // ---------
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
    std::shared_ptr<GameObject> optionsBackground = GameObject::Instantiate("Background1", options);
    optionsBackground->AddComponent<Image>();
    optionsBackground->GetComponent<Image>()->LoadTexture(0, 0, "UI/options.png");
    std::shared_ptr<GameObject> backToPauseMenu = GameObject::Instantiate("BackToPauseMenu", options);
    backToPauseMenu->AddComponent<Button>();
    backToPauseMenu->GetComponent<Button>()->LoadTexture(900, 500, "UI/button.png", "UI/activeButton.png");
    backToPauseMenu->GetComponent<Button>()->LoadFont("Back", 905, 520);
    std::shared_ptr<GameObject> decreaseVolume = GameObject::Instantiate("DecreaseVolume", options);
    decreaseVolume->AddComponent<Button>();
    decreaseVolume->GetComponent<Button>()->LoadTexture(1000, 800, "UI/button.png", "UI/activeButton.png");
    decreaseVolume->GetComponent<Button>()->LoadFont("-", 1015, 810, 64);
    std::shared_ptr<GameObject> increaseVolume = GameObject::Instantiate("IncreaseVolume", options);
    increaseVolume->AddComponent<Button>();
    increaseVolume->GetComponent<Button>()->LoadTexture(1100, 800, "UI/button.png", "UI/activeButton.png");
    increaseVolume->GetComponent<Button>()->LoadFont("+", 1115, 810, 64);
    std::shared_ptr<GameObject> decreaseResolution = GameObject::Instantiate("DecreaseResolution", options);
    decreaseResolution->AddComponent<Button>();
    decreaseResolution->GetComponent<Button>()->LoadTexture(1000, 700, "UI/button.png", "UI/activeButton.png");
    std::shared_ptr<GameObject> increaseResolution = GameObject::Instantiate("IncreaseResolution", options);
    increaseResolution->AddComponent<Button>();
    increaseResolution->GetComponent<Button>()->LoadTexture(1100, 700, "UI/button.png", "UI/activeButton.png");
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


    std::shared_ptr<GameObject> sphere = GameObject::Instantiate("Sphere", activeScene);
    sphere->transform->SetLocalPosition({0, 2, 0});
    sphere->AddComponent<Renderer>()->LoadModel("sphere/sphere.obj");

    std::shared_ptr<GameObject> lowPolyHouse = GameObject::Instantiate("LowPolyHouse", activeScene);
    lowPolyHouse->transform->SetLocalPosition({10, 0, -20});
    lowPolyHouse->transform->SetLocalRotation({0, 0, 0});
    lowPolyHouse->transform->SetLocalScale({4, 4, 4});
    lowPolyHouse->AddComponent<Renderer>()->LoadModel("OBJ/Shop.obj");

	// Set up animated model
	std::shared_ptr<GameObject> animatedDood = Prefab::GetDancingDude();
	animatedDood->transform->SetLocalPosition({0, 0, -4});
	animatedDood->transform->SetLocalScale({5, 5, 5});

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    return false;
}
