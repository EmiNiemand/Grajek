#include "Game.h"
#include "GloomEngine.h"
#include "EngineManagers/HIDManager.h"
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
#include "Components/Scripts/PlayerMovement.h"
#include "Components/Renderers/Animator.h"

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

    // Set up player
    // -------------
    std::shared_ptr<GameObject> player = GameObject::Instantiate("Player", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> playerRenderer = player->AddComponent<Renderer>();
    playerRenderer->LoadModel("domek/domek.obj");
    std::shared_ptr<Rigidbody> cubeRigidbody = player->AddComponent<Rigidbody>();
    player->AddComponent<PlayerMovement>();
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

    // Set up cubemap
    // --------------
    auto sky = GameObject::Instantiate("CubeMap", activeScene);
    auto skyCubeMap = sky->AddComponent<CubeMap>();
    skyCubeMap->LoadTextures("skybox/");


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
	std::shared_ptr<BoxCollider> scenePropCollider = animatedDood->AddComponent<BoxCollider>();
	scenePropCollider->SetOffset({0, 1, 0});
	animatedDood->transform->SetLocalPosition({0, 2, -5});
	animatedDood->transform->SetLocalScale({5, 5, 5});
	animatedDoodAnimator->modelMatrix = animatedDood->transform->GetModelMatrix();

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::Update() {
    return false;
}
