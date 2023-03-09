#include "include/GloomEngine.h"
#include "include/EngineComponents/EngineRenderer.h"
#include "include/EngineComponents/EngineColliders.h"
#include "include/Factiories/GameObjectFactory.h"
#include "include/Factiories/ComponentFactory.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Component.h"
#include "include/Components/Renderer.h"
#include "include/Components/Camera.h"
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
    engineColliders = std::make_unique<EngineColliders>(shared_from_this(), false);

    // INIT FACTORIES
    gameObjectFactory = std::make_unique<GameObjectFactory>(shared_from_this());
    componentFactory = std::make_unique<ComponentFactory>(shared_from_this());

    // CREATE BASIC SCENE
    activeScene = gameObjectFactory->CreateGameObject("Scene", nullptr, Tags::SCENE);

    // TODO: delete this
    activeCamera = gameObjectFactory->CreateGameObject("Camera", activeScene, Tags::CAMERA);
    componentFactory->CreateCamera(activeCamera);
    activeCamera->transform->SetLocalPosition({0, 20, 10});
    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(activeCamera->FindComponentByName(ComponentNames::CAMERA));

    std::shared_ptr<GameObject> cube = gameObjectFactory->CreateGameObject("Cube", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> cubeRenderer = componentFactory->CreateRenderer(cube);
    cubeRenderer->LoadModel("res/models/domek/domek.obj");
    std::shared_ptr<BoxCollider> cubeCollider = componentFactory->CreateBoxCollider(cube);
    cube->transform->SetLocalPosition({0, 0, -10});
    cube->transform->SetLocalScale({10, 0.1, 6});

    std::shared_ptr<GameObject> cube1 = gameObjectFactory->CreateGameObject("Cube", activeScene, Tags::DEFAULT);
    std::shared_ptr<Renderer> cubeRenderer1 = componentFactory->CreateRenderer(cube1);
    cubeRenderer1->LoadModel("res/models/domek/domek.obj");
    std::shared_ptr<BoxCollider> cubeCollider1 = componentFactory->CreateBoxCollider(cube1);
    cubeCollider1->SetOffset({0, 1, 0});
    cube1->transform->SetLocalPosition({0, 10, -10});
    cube1->transform->SetLocalScale({0.5, 1, 0.5});

    std::shared_ptr<GameObject> cube2 = gameObjectFactory->CreateGameObject("Cube", cube1, Tags::DEFAULT);
    std::shared_ptr<SpotLight> spotLight2 = componentFactory->CreateSpotLight(cube2);
    cube2->transform->SetLocalPosition({0, 10, -10});
    cube2->transform->SetLocalRotation({-90, 0, 0});

    std::shared_ptr<GameObject> cube3 = gameObjectFactory->CreateGameObject("Cube", cube1, Tags::DEFAULT);;
    std::shared_ptr<PointLight> pointLight2 = componentFactory->CreatePointLight(cube3);
    cube3->transform->SetLocalPosition({0, 1, -10});

    camera->SetTarget(cube3);
}

void GloomEngine::Awake() {
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
    deltaTime = currentTime - lastFrameTime;

    engineRenderer->UpdateRenderer();

    std::shared_ptr<GameObject> cube = FindGameObjectWithName("Cube1");
    cube->transform->SetLocalPosition(cube->transform->GetLocalPosition() + glm::vec3(0, -0.1, 0));

    engineColliders->Update();

    for (auto&& component : components){
        if (component.second->callOnAwake) component.second->Awake();
        if (component.second->callOnStart) component.second->Start();
        if (component.second->enabled) component.second->Update();
    }

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
    if (component->GetName() == ComponentNames::SPOTLIGHT || component->GetName() == ComponentNames ::DIRECTIONALLIGHT ||
        component->GetName() == ComponentNames::POINTLIGHT) {
        engineRenderer->RemoveLight(componentId);
    }
    if (component->GetName() == ComponentNames::BOXCOLLIDER) engineColliders->RemoveBoxCollider(componentId);
    components.erase(componentId);
}

std::shared_ptr<GameObject> GloomEngine::FindGameObjectWithId(int id) {
    if(!gameObjects.contains(id)) return nullptr;
    return gameObjects.find(id)->second;
}

std::shared_ptr<GameObject> GloomEngine::FindGameObjectWithName(std::string name) {
    for (auto&& object : gameObjects){
        if (object.second->GetName() == name) return object.second;
    }
    return nullptr;
}

