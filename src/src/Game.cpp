#include "include/Game.h"
#include "include/GloomEngine.h"
#include "include/EngineManagers/RendererManager.h"
#include "include/EngineManagers/ColliderManager.h"
#include "include/EngineManagers/HIDManager.h"
#include "include/EngineManagers/SceneManager.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"
#include "include/Components/Renderers/Renderer.h"
#include "include/Components/Renderers/Camera.h"
#include "include/Components/Renderers/Lights/PointLight.h"
#include "include/Components/Renderers/Lights/DirectionalLight.h"
#include "include/Components/Renderers/Lights/SpotLight.h"
#include "include/Components/PhysicsAndColliders/Rigidbody.h"
#include "include/Components/PhysicsAndColliders/BoxCollider.h"
#include "include/Components/Scripts/PlayerMovement.h"

Game::Game(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {
    activeCamera = Camera::activeCamera;
    activeScene = gloomEngine->sceneManager->activeScene;
}

Game::~Game() {}

void Game::InitializeGame() {
    std::shared_ptr<Camera> camera = activeCamera->AddComponent<Camera>();
    camera->cameraOffset = glm::vec3(0, 20, 20);
    camera->parameter = 0.02f;
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

    std::shared_ptr<GameObject> cube = GameObject::Instantiate("Cube", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> cubeRenderer = cube->AddComponent<Renderer>();
    cubeRenderer->LoadModel("domek/domek.obj");
    std::shared_ptr<BoxCollider> cubeCollider = cube->AddComponent<BoxCollider>();
    cubeCollider->SetOffset({0, 1, 0});
    cube->transform->SetLocalPosition({0, -4, -10});
    cube->transform->SetLocalScale({20, 2, 20});

    std::shared_ptr<GameObject> sun = GameObject::Instantiate("Sun", activeScene);
    sun->AddComponent<PointLight>();
    sun->transform->SetLocalPosition({25, 100, 25});

    for (int i = 0; i < 10; i++) {
        std::shared_ptr<GameObject> cube5 = GameObject::Instantiate("Cube", activeScene, Tags::DEFAULT);
        std::shared_ptr<Renderer> cubeRenderer5 = cube5->AddComponent<Renderer>();
        cubeRenderer5->LoadModel("domek/domek.obj");
        std::shared_ptr<BoxCollider> cubeCollider5 = cube5->AddComponent<BoxCollider>();
        cubeCollider5->SetOffset({0, 1, 0});
        cube5->transform->SetLocalPosition({i * std::cos(i) * 10, 0, -20 + i * std::sin(i)});
        cube5->transform->SetLocalRotation({0, cos(i) * 90, 0});
    }

    //camera->SetTarget(pivot);
    camera->SetTarget(nullptr);
}

bool Game::Update() {
    return false;
}
