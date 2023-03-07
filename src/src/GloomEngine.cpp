#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/EngineComponents/EngineColliders.h"
#include "include/Factiories/GameObjectFactory.h"
#include "include/Factiories/ComponentFactory.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Component.h"
#include "include/Components/Renderer.h"
#include "include/Components/Lights/PointLight.h"
#include "include/Components/Lights/DirectionalLight.h"
#include "include/Components/Lights/SpotLight.h"
#include "include/Components/BoxCollider.h"

GloomEngine::GloomEngine(GLFWwindow* window, int *width, int *height) : window(window), width(width), height(height) {

}

GloomEngine::~GloomEngine() {

}

void GloomEngine::Init() {
    //INIT ENGINE COMPONENTS
    engineRenderer = std::make_unique<EngineRenderer>(shared_from_this());
    engineColliders = std::make_unique<EngineColliders>(shared_from_this(), true);

    // INIT FACTORIES
    gameObjectFactory = std::make_unique<GameObjectFactory>(shared_from_this());
    componentFactory = std::make_unique<ComponentFactory>(shared_from_this());

    // CREATE BASIC SCENE
    activeScene = gameObjectFactory->CreateGameObject("Scene", nullptr, Tags::SCENE);

    // TODO: delete this
    activeCamera = gameObjectFactory->CreateGameObject("Camera", activeScene, Tags::CAMERA);
    componentFactory->CreateCamera(activeCamera);
    activeCamera->transform->SetLocalPosition({0, 1, 0});
    activeCamera->UpdateSelfAndChildren();

    std::shared_ptr<GameObject> cube = gameObjectFactory->CreateGameObject("Cube", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> cubeRenderer = componentFactory->CreateRenderer(cube);
    cubeRenderer->LoadModel("res/models/domek/domek.obj");
    std::shared_ptr<BoxCollider> cubeCollider = componentFactory->CreateBoxCollider(cube);
    cubeCollider->SetOffset({0, 1, 0});
    cube->transform->SetLocalPosition({0, 0, -10});
    cube->transform->SetLocalScale({1, 1, 1});

    std::shared_ptr<GameObject> cube1 = gameObjectFactory->CreateGameObject("Cube", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> cubeRenderer1 = componentFactory->CreateRenderer(cube1);
    cubeRenderer1->LoadModel("res/models/domek/domek.obj");
    std::shared_ptr<BoxCollider> cubeCollider1 = componentFactory->CreateBoxCollider(cube1);
    cubeCollider1->SetOffset({0, 1, 0});
    cube1->transform->SetLocalPosition({0, 10, -10});
    cube1->transform->SetLocalScale({1, 1, 1});
    cube1->transform->SetLocalRotation({0, 30, 0});

    std::shared_ptr<GameObject> light = gameObjectFactory->CreateGameObject("Light", activeScene, Tags::LIGHT);
    componentFactory->CreateDirectionalLight(light);

}

void GloomEngine::Awake() {
    // Setup all engine components
    engineRenderer->UpdateRenderer();

    for (auto&& component : components){
        component->Awake();
    }
}

void GloomEngine::Start() {
    for (auto&& component : components){
        if (component->enabled) component->Start();
    }
}

bool GloomEngine::Update() {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;


    std::shared_ptr<GameObject> cube1 = gameObjects.find("Cube1")->second;

    cube1->transform->SetLocalPosition(cube1->transform->GetLocalPosition() - glm::vec3({0, 0.1, 0}));

    for (auto&& component : components){
        if (component->callOnAwake) component->Awake();
        if (component->callOnStart) component->Start();
        if (component->enabled) component->Update();
    }

    engineColliders->Update();

    // TODO: add way to get out of the game

    lastFrameTime = currentTime;
    return false;
}

void GloomEngine::Destroy() {
    ClearScene();
    engineColliders->Destroy();
    engineRenderer->Destroy();
    gameObjectFactory = nullptr;
    componentFactory = nullptr;
    activeScene = nullptr;
}

void GloomEngine::ClearScene() {
    activeScene->RemoveAllChildren();

    components.clear();
    gameObjects.clear();
}

void GloomEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.insert({gameObject->GetName(), gameObject});
}

void GloomEngine::AddComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
}

void GloomEngine::RemoveGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.erase(gameObject->GetName());
}

void GloomEngine::RemoveComponent(std::shared_ptr<Component> component) {
    for (int i = 0; i < components.size(); i++){
        if (components[i] == component) {
            components.erase(components.begin() + i);
            break;
        }
    }
}

std::shared_ptr<GameObject> GloomEngine::FindGameObjectWithName(std::string name) {
    if(!gameObjects.contains(name)) return nullptr;
    return gameObjects.find(name)->second;
}

