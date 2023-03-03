#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/Factiories/GameObjectFactory.h"
#include "include/Factiories/ComponentFactory.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Component.h"
#include "include/Components/Renderer.h"
#include "include/Components/Lights/PointLight.h"
#include "include/Components/Lights/DirectionalLight.h"
#include "include/Components/Lights/SpotLight.h"

GloomEngine::GloomEngine(GLFWwindow* window, int *width, int *height) : window(window), width(width), height(height) {

}

GloomEngine::~GloomEngine() {

}

void GloomEngine::Init() {
    //INIT ENGINE COMPONENTS
    engineRenderer = std::make_unique<EngineRenderer>(shared_from_this());

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
    cube->transform->SetLocalPosition({0, 0, -4});
    cube->transform->SetLocalScale({0.5, 0.5, 0.5});

    std::shared_ptr<GameObject> cube2 = gameObjectFactory->CreateGameObject("Cube", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> cubeRenderer2 = componentFactory->CreateRenderer(cube2);
    cubeRenderer2->LoadModel("res/models/domek/domek.obj");
    cube2->transform->SetLocalPosition({-2, 0, -6});
    cube2->transform->SetLocalScale({0.5, 0.5, 0.5});

    std::shared_ptr<GameObject> light = gameObjectFactory->CreateGameObject("Light", activeScene, Tags::LIGHT);
    componentFactory->CreateDirectionalLight(light);
    light->transform->SetLocalRotation({0, 90, 0});

    std::shared_ptr<GameObject> light2 = gameObjectFactory->CreateGameObject("Light", activeScene, Tags::LIGHT);
    std::shared_ptr<Renderer> light2Renderer = componentFactory->CreateRenderer(light2);
    light2Renderer->LoadModel("res/models/domek/domek.obj");
    componentFactory->CreatePointLight(light2);
    light2->transform->SetLocalPosition({1.5, 1.5, -9});
    light2->transform->SetLocalScale({0.1, 0.1, 0.1});

    std::shared_ptr<GameObject> light3 = gameObjectFactory->CreateGameObject("Light", activeScene, Tags::LIGHT);
    std::shared_ptr<Renderer> light3Renderer = componentFactory->CreateRenderer(light3);
    light3Renderer->LoadModel("res/models/domek/domek.obj");
    componentFactory->CreateSpotLight(light3);
    light3->transform->SetLocalPosition({-1, 0.5, -2});
    light3->transform->SetLocalScale({0.1, 0.1, 0.1});

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


    for (auto&& component : components){
        if (component->callOnAwake) component->Awake();
        if (component->callOnStart) component->Start();
        if (component->enabled) component->Update();
    }


    // TODO: add way to get out of the game

    lastFrameTime = currentTime;
    return false;
}

void GloomEngine::Destroy() {
    ClearScene();
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

