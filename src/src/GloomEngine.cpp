#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/EngineComponents/EngineColliders.h"
#include "include/EngineComponents/EngineHID.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"
#include "include/Components/Renderers/Renderer.h"
#include "include/Components/Renderers/Camera.h"
#include "include/Components/Renderers/Lights/PointLight.h"
#include "include/Components/Renderers/Lights/DirectionalLight.h"
#include "include/Components/Renderers/Lights/SpotLight.h"
#include "include/Components/PhysicsAndColliders/Rigidbody.h"
#include "include/Components/PhysicsAndColliders/BoxCollider.h"
#include "include/Components/Scripts/PlayerMovement.h"

GloomEngine::GloomEngine(GLFWwindow* window, int *width, int *height) : window(window), width(width), height(height) {

}

GloomEngine::~GloomEngine() {

}

void GloomEngine::Init() {
    // Assign engin as parent to game objects
    GameObject::Init(shared_from_this());

    //INIT ENGINE COMPONENTS
    engineRenderer = std::make_unique<EngineRenderer>(shared_from_this());
    engineColliders = std::make_unique<EngineColliders>(shared_from_this(), false);
    engineHID = std::make_unique<EngineHID>(shared_from_this());


    // CREATE BASIC SCENE
    activeScene = GameObject::Instantiate("Scene", nullptr, Tags::SCENE);

    // TODO: delete this
    activeCamera = GameObject::Instantiate("Camera", activeScene, Tags::CAMERA);
    std::shared_ptr<Camera> camera = activeCamera->AddComponent<Camera>();
    activeCamera->transform->SetLocalPosition({0, 20, 10});

    std::shared_ptr<GameObject> player = GameObject::Instantiate("Player", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> playerRenderer = player->AddComponent<Renderer>();
    playerRenderer->LoadModel("domek/domek.obj");
    std::shared_ptr<Rigidbody> cubeRigidbody = player->AddComponent<Rigidbody>();
    player->AddComponent<PlayerMovement>();
    player->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    player->transform->SetLocalPosition({0, 30, -10});
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

    camera->SetTarget(pivot);
}

void GloomEngine::Awake() {
    lastFrameTime = glfwGetTime();
    // Setup all engine components
    engineRenderer->UpdateRenderer();

    for (auto&& component : components){
        component.second->Awake();
    }
}

void GloomEngine::Start() {
    for (auto&& component : components){
        if (component.second->enabled) component.second->Start();
    }
}

bool GloomEngine::Update() {
    float currentTime = glfwGetTime();

    for (auto&& component : components){
        if (component.second->callOnAwake) component.second->Awake();
        if (component.second->callOnStart) component.second->Start();
        if (component.second->enabled) component.second->Update();
    }

    engineHID->Update();
    engineColliders->Update();
    engineRenderer->UpdateRenderer();

    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    timer += deltaTime;
    frames++;

    if (timer >= 1) {
//        spdlog::info(frames);
        frames = 0;
        timer = 0;
    }

    // TODO: add way to get out of the game
    return false;
}

void GloomEngine::Free() {
    ClearScene();
    engineColliders->Free();
    engineRenderer->Free();
    activeScene = nullptr;
}

void GloomEngine::ClearScene() {
    activeScene->RemoveAllChildren();

    components.clear();
    gameObjects.clear();
}

//DO NOT USE
void GloomEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.insert({gameObject->GetId(), gameObject});
}

void GloomEngine::AddComponent(std::shared_ptr<Component> component) {
    components.insert({component->GetId(), component});
}

void GloomEngine::OnUpdate(int componentId) {
    engineRenderer->UpdateLight(componentId);
}

void GloomEngine::RemoveGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.erase(gameObject->GetId());
}

void GloomEngine::RemoveComponent(std::shared_ptr<Component> component) {
    int componentId = component->GetId();
    engineRenderer->RemoveLight(componentId);
    engineColliders->RemoveBoxCollider(componentId);
    components.erase(componentId);
}

std::shared_ptr<GameObject> GloomEngine::FindGameObjectWithId(int id) {
    if(!gameObjects.contains(id)) return nullptr;
    return gameObjects.find(id)->second;
}

std::shared_ptr<GameObject> GloomEngine::FindGameObjectWithName(std::string name) {
    if (!gameObjects.empty()) {
        for (auto&& object : gameObjects) {
            if (object.second->GetName() == name) return object.second;
        }
    }
    return nullptr;
}

