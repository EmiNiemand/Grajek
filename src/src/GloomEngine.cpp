#include "GloomEngine.h"
#include "Game.h"
#include "EngineManagers/RendererManager.h"
#include "EngineManagers/ColliderManager.h"
#include "EngineManagers/HIDManager.h"
#include "EngineManagers/SceneManager.h"
#include "EngineManagers/DebugManager.h"
#include "EngineManagers/DataPersistanceManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/PlayerMovement.h"

#include <filesystem>

GloomEngine::GloomEngine() {
    width = 1200;
    height = 780;
}

GloomEngine::~GloomEngine() = default;

GloomEngine* GloomEngine::GetInstance() {
    if (gloomEngine == nullptr) {
        gloomEngine = new GloomEngine();
    }
    return gloomEngine;
}


void GloomEngine::Initialize() {
    InitializeWindow();

    SceneManager::GetInstance()->InitializeScene();
    RendererManager::GetInstance()->UpdateProjection();

    game = std::make_shared<Game>();
    game->InitializeGame();

    // Load game
    std::filesystem::path path = std::filesystem::current_path();
    DataPersistanceManager::GetInstance()->LoadGame(path.string(), "Save1");
}

void GloomEngine::Awake() {
    lastFrameTime = (float)glfwGetTime();
    lastFixedFrameTime = (float)glfwGetTime();
    lastAIFrameTime = (float)glfwGetTime();

    for (auto&& component : components) {
        if (component.second->callOnAwake) component.second->Awake();
    }
}

void GloomEngine::Start() {
    for (auto&& component : components){
        if (component.second->enabled && component.second->callOnStart) component.second->Start();
    }
}

bool GloomEngine::MainLoop() {
    auto currentTime = (float)glfwGetTime();

    glfwPollEvents();
    glfwSetWindowSize(window, width, height);

    int multiplier60Rate = (int)((currentTime - (float)(int)currentTime) * 60);
    int multiplier60LastRate = (int)((lastFrameTime - (float)(int)lastFrameTime) * 60);
    if (multiplier60Rate > multiplier60LastRate || (multiplier60Rate == 0 && multiplier60LastRate != 0)) {
        glfwMakeContextCurrent(window);
        glViewport(0, 0, width, height);
        glClearColor(screenColor.x, screenColor.y, screenColor.z, screenColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Update();

        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    int multiplier120Rate = (int)((currentTime - (float)(int)currentTime) * 120);
    int multiplier120LastRate = (int)((lastFixedFrameTime - (float)(int)lastFixedFrameTime) * 120);
    if (multiplier120Rate > multiplier120LastRate || (multiplier120Rate == 0 && multiplier120LastRate != 0)) {

        FixedUpdate();

        fixedDeltaTime = currentTime - lastFixedFrameTime;
        lastFixedFrameTime = currentTime;
    }

    int multiplier2Rate = (int)((currentTime - (float)(int)currentTime) * 2);
    int multiplier2LastRate = (int)((lastAIFrameTime - (float)(int)lastAIFrameTime) * 2);
    if (multiplier2Rate > multiplier2LastRate || (multiplier2Rate == 0 && multiplier2LastRate != 0)) {

        AIUpdate();

        AIDeltaTime = currentTime - lastAIFrameTime;
        lastAIFrameTime = currentTime;
    }

    bool endGame = game->GameLoop();

    // Save game on quit
    if (glfwWindowShouldClose(window) || endGame) {
        std::filesystem::path path = std::filesystem::current_path();
        DataPersistanceManager::GetInstance()->SaveGame(path.string(), "Save1");
    }
    return glfwWindowShouldClose(window) || endGame;
}

void GloomEngine::Update() {
    for (auto&& component : components) {
        if (component.second->callOnAwake) component.second->Awake();
        if (component.second->callOnStart && component.second->enabled) component.second->Start();
        if (component.second->enabled) component.second->Update();
    }

    RendererManager::GetInstance()->DrawObjects();

#ifdef DEBUG
    ColliderManager::GetInstance()->DrawColliders();
    DebugManager::GetInstance()->Render();
#endif

    HIDManager::GetInstance()->ManageInput();
}

void GloomEngine::FixedUpdate() {
    for (auto&& component : components) {
        if (component.second->enabled) component.second->FixedUpdate();
    }

    ColliderManager::GetInstance()->ManageCollision();
}

void GloomEngine::AIUpdate() {
    for (auto&& component : components) {
        if (component.second->enabled) component.second->AIUpdate();
    }
}

void GloomEngine::Free() const {
    ColliderManager::GetInstance()->Free();
    RendererManager::GetInstance()->Free();
    SceneManager::GetInstance()->Free();
    glfwDestroyWindow(window);
    glfwTerminate();
}

std::shared_ptr<GameObject> GloomEngine::FindGameObjectWithId(int id) {
    if(!gameObjects.contains(id)) return nullptr;
    return gameObjects.find(id)->second;
}

std::shared_ptr<GameObject> GloomEngine::FindGameObjectWithName(const std::string& name) {
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
    glfwSwapInterval(false);

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

#ifdef DEBUG
    DebugManager::GetInstance()->Initialize(window, glsl_version);
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GloomEngine::AddGameObject(const std::shared_ptr<GameObject>& gameObject) {
    gameObjects.insert({gameObject->GetId(), gameObject});
}

void GloomEngine::AddComponent(const std::shared_ptr<Component>& component) {
    components.insert({component->GetId(), component});
}

void GloomEngine::RemoveGameObject(const std::shared_ptr<GameObject>& gameObject) {
    gameObjects.erase(gameObject->GetId());
}

void GloomEngine::RemoveComponent(const std::shared_ptr<Component>& component) {
    int componentId = component->GetId();
    components.erase(componentId);
}

void GloomEngine::glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
