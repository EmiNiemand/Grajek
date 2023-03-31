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
    std::shared_ptr<GameObject> player = GameObject::Instantiate("Player", activeScene, Tags::DEFAULT);
    // Adding components
    std::shared_ptr<Renderer> playerRenderer = player->AddComponent<Renderer>();
    playerRenderer->LoadModel("domek/domek.obj");
    playerRenderer->material.reflection = 0.5f;
    std::shared_ptr<Rigidbody> cubeRigidbody = player->AddComponent<Rigidbody>();
    player->AddComponent<PlayerMovement>();
    player->AddComponent<PlayerManager>();
    // Setting values
    player->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    player->transform->SetLocalPosition({0, 2, -10});
    player->transform->SetLocalScale({0.5, 1, 0.5});
    std::shared_ptr<GameObject> pivot = GameObject::Instantiate("Cube", player, Tags::DEFAULT);;
    pivot->transform->SetLocalPosition({0, 1, -10});

    // Set up ground
    // -------------
    std::shared_ptr<GameObject> ground = GameObject::Instantiate("Cube", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> groundRenderer = ground->AddComponent<Renderer>();
    groundRenderer->LoadModel("domek/domek.obj");
    std::shared_ptr<BoxCollider> groundCollider = ground->AddComponent<BoxCollider>();
    groundCollider->SetOffset({0, 1, 0});
    ground->transform->SetLocalPosition({0, -4, -10});
    ground->transform->SetLocalScale({20, 2, 20});

    // Set up lights
    // -------------
    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<PointLight>();
    sun->transform->SetLocalPosition({25, 100, 25});

    // Set up UI
    // ---------
    std::shared_ptr<GameObject> tekst = GameObject::Instantiate("Tekst", activeScene);
    tekst->AddComponent<Text>();
    // x,y from 0 to 1920
    tekst->GetComponent<Text>()->LoadFont("easter egg", 1725, 10, 18, glm::vec3(1.0f, 1.0f, 1.0f), "Eggnog.ttf");
    std::shared_ptr<GameObject> reksio = GameObject::Instantiate("Reksio", activeScene);
    reksio->AddComponent<Image>();
    // x,y from 0 to 1920
    reksio->GetComponent<Image>()->LoadTextures(50, 0, "UI/piesek.png");
    std::shared_ptr<GameObject> mruczek = GameObject::Instantiate("Mruczek", activeScene);
    mruczek->AddComponent<Image>();
    mruczek->GetComponent<Image>()->LoadTextures(1650, 0, "UI/kotek.png");



    // Set up cubes for collision testing
    // ----------------------------------
    for (int i = 0; i < 10; i++) {
        std::shared_ptr<GameObject> sceneProp = GameObject::Instantiate("Cube", activeScene, Tags::DEFAULT);
        std::shared_ptr<Renderer> scenePropRenderer = sceneProp->AddComponent<Renderer>();
        scenePropRenderer->LoadModel("domek/domek.obj");
        std::shared_ptr<BoxCollider> scenePropCollider = sceneProp->AddComponent<BoxCollider>();
        scenePropCollider->SetOffset({0, 1, 0});
        sceneProp->transform->SetLocalPosition({i * std::cos(i) * 10, 0, -20 + i * std::sin(i)});
        sceneProp->transform->SetLocalRotation({0, cos(i) * 90, 0});
    }

	// Set up animated model
	std::shared_ptr<GameObject> animatedDood = GameObject::Instantiate("dood", activeScene, Tags::DEFAULT);
	std::shared_ptr<Animator> animatedDoodAnimator = animatedDood->AddComponent<Animator>();
	animatedDoodAnimator->LoadAnimation("hiphopnigdystop/HipHopDancing.dae");
	animatedDood->transform->SetLocalPosition({0, 0, -25});
	animatedDood->transform->SetLocalScale({5, 5, 5});

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::GameLoop() {
    return false;
}
