#include "include/GloomEngine.h"
#include "include/Game.h"
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

GloomEngine::GloomEngine() {
    width = 1200;
    height = 780;
}

GloomEngine::~GloomEngine() {}

void GloomEngine::Init() {
    InitializeWindow();

    // Assign engin as parent to game objects
    GameObject::Init(shared_from_this());

    //INIT ENGINE COMPONENTS
    engineRenderer = std::make_unique<EngineRenderer>(shared_from_this());
    engineColliders = std::make_unique<EngineColliders>(shared_from_this());
    engineHID = std::make_unique<EngineHID>(shared_from_this());

    game = std::make_shared<Game>(shared_from_this());
    game->Init();
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

    glfwPollEvents();
    glfwSetWindowSize(window, width, height);

    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
    glClearColor(screenColor.x, screenColor.y, screenColor.z, screenColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto&& component : components){
        if (component.second->callOnAwake) component.second->Awake();
        if (component.second->callOnStart) component.second->Start();
        if (component.second->enabled) component.second->Update();
    }

    bool endGame = game->Update();

    engineHID->Update();
    engineColliders->Update();
    engineRenderer->UpdateRenderer();

    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);

    return glfwWindowShouldClose(window) || endGame;
}

void GloomEngine::Free() {
    ClearScene();
    engineColliders->Free();
    engineRenderer->Free();
    activeScene = nullptr;
    glfwDestroyWindow(window);
    glfwTerminate();
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

// PRIVATE FUNCTIONS
void GloomEngine::InitializeWindow() {
    // Setup window
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        throw;

    // Decide GL+GLSL versions
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    window = glfwCreateWindow(width, height, "Gloomies", NULL, NULL);
    if (window == nullptr)
        throw;
    glfwMakeContextCurrent(window);
    // Enable vsync
    glfwSwapInterval(true);

    // Center window on the screen
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int monitorWidth = mode->width;
    int monitorHeight = mode->height;
    glfwSetWindowPos(window, monitorWidth / 2 - width / 2, monitorHeight / 2 - height / 2);

    // Enable cursor - change last parameter to disable it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
    if (err)
    {
        spdlog::error("Failed to initialize OpenGL loader!");
        throw;
    }
    spdlog::info("Successfully initialized OpenGL loader!");


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void GloomEngine::RemoveGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.erase(gameObject->GetId());
}

void GloomEngine::RemoveComponent(std::shared_ptr<Component> component) {
    int componentId = component->GetId();
    engineRenderer->RemoveLight(componentId);
    engineColliders->RemoveBoxCollider(componentId);
    components.erase(componentId);
}

void GloomEngine::glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

