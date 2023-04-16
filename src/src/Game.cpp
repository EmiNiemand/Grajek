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
#include "Components/Renderers/Animator.h"
#include "Components/UI/Image.h"
#include "Components/UI/Text.h"
#include "Components/UI/Button.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"

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
    tekst->GetComponent<Text>()->LoadFont("easter egg", 1725, 10, 18, glm::vec3(1.0f, 1.0f, 1.0f), "Eggnog.ttf");
    std::shared_ptr<GameObject> pause = GameObject::Instantiate("Pause", activeScene);
    std::shared_ptr<GameObject> pauseBackground = GameObject::Instantiate("Background", pause);
    pauseBackground->AddComponent<Image>();
    pauseBackground->GetComponent<Image>()->LoadTexture(0, 0, "UI/pause.png");
    std::shared_ptr<GameObject> button = GameObject::Instantiate("Button", pause);
    button->AddComponent<Button>();
    button->GetComponent<Button>()->LoadTexture(900, 500, "UI/button.png", "UI/activeButton.png");
    button->GetComponent<Button>()->LoadFont("Exit", 905, 520, 18, glm::vec3(0.0f, 0.0f, 0.0f), "Eggnog.ttf");
    pause->DisableSelfAndChildren();


    std::shared_ptr<GameObject> sphere = GameObject::Instantiate("Sphere", activeScene);
    sphere->transform->SetLocalPosition({-5, 2, 0});
    sphere->transform->SetLocalScale({2, 2, 2});
    sphere->AddComponent<Renderer>()->LoadModel("sphere/sphere.obj");
    std::shared_ptr<Renderer> rSphere = sphere->GetComponent<Renderer>();
    rSphere->material.refraction = 1.0f;

    std::shared_ptr<GameObject> lowPolyHouse = GameObject::Instantiate("LowPolyHouse", activeScene);
    lowPolyHouse->transform->SetLocalPosition({10, 0, -20});
    lowPolyHouse->transform->SetLocalRotation({0, 0, 0});
    lowPolyHouse->transform->SetLocalScale({4, 4, 4});
    lowPolyHouse->AddComponent<Renderer>()->LoadModel("OBJ/Shop.obj");
    auto portalRadioSound = lowPolyHouse->AddComponent<AudioSource>();
    portalRadioSound->LoadAudioData("res/sounds/portal_radio.wav", AudioType::Sound);
    portalRadioSound->SetPositionOffset({0.0f, 0.0f, 2.0f});
    portalRadioSound->SetDistanceMode(AudioDistanceMode::Continuous);
    portalRadioSound->SetMaxDistance(20.0f);
    portalRadioSound->SetCone({0.0f, 0.0f, 1.0f}, {110.0f, 200.0f});
    portalRadioSound->IsLooping(true);
    portalRadioSound->PlaySound();

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
