#include "include/GloomEngine.h"
#include "include/Components/Component.h"
#include "include/HighLevelClasses/GameObject.h"
#include "include/Factiories/GameObjectFactory.h"
#include "include/Factiories/ComponentFactory.h"
#include "include/Components/Renderer.h"

GloomEngine::GloomEngine(GLFWwindow* window, int *width, int *height) : window(window), width(width), height(height) {
}

GloomEngine::~GloomEngine() {

}

void GloomEngine::Init() {
    scene = std::make_shared<GameObject>(shared_from_this(), "Scene");
    gameObjectFactory = std::make_unique<GameObjectFactory>(shared_from_this());
    componentFactory = std::make_unique<ComponentFactory>(shared_from_this());

    // TODO: delete this
    activeCamera = gameObjectFactory->CreateGameObject("Camera", scene, Tags::CAMERA);
    componentFactory->CreateCamera(activeCamera);
    activeCamera->transform.SetLocalPosition({0, 1, 0});
    activeCamera->UpdateSelfAndChildren();
    std::shared_ptr<GameObject> cube = gameObjectFactory->CreateGameObject("Cube", scene, Tags::DEFAULT);
    std::shared_ptr<Renderer> cubeRenderer = componentFactory->CreateRenderer(cube);
    cubeRenderer->LoadModel("res/models/domek/domek.obj");
    cube->transform.SetLocalPosition({0, 0, -4});
    cube->transform.SetLocalScale({1, 1, 1});
    cube->UpdateSelfAndChildren();
}

void GloomEngine::Awake() {
    for (auto&& component : components){
        component->Awake();
    }
}

void GloomEngine::Start() {
    for (auto&& component : components){
        component->Start();
    }
}

bool GloomEngine::Update() {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;

    for (auto&& component : components){
        component->Update();
    }


    // TODO: add way to get out of the game

    lastFrameTime = currentTime;
    return false;
}

void GloomEngine::Destroy() {
    ClearScene();
    gameObjectFactory = nullptr;
    componentFactory = nullptr;
    scene = nullptr;
}

void GloomEngine::ClearScene() {
    scene->RemoveAllChildren();

    components.clear();
    gameObjects.clear();
}

void GloomEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.insert({gameObject->GetName(), gameObject});
}

void GloomEngine::AddComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
}

