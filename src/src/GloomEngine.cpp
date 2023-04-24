#include "GloomEngine.h"
#include "Game.h"
#include "EngineManagers/AudioManager.h"
#include "EngineManagers/RendererManager.h"
#include "EngineManagers/PostProcessingManager.h"
#include "EngineManagers/UIManager.h"
#include "EngineManagers/CollisionManager.h"
#include "EngineManagers/ShadowManager.h"
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
#include "Other/FrustumCulling.h"

#include <filesystem>

GloomEngine::GloomEngine() {
    width = 1440;
    height = 810;
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
    AudioManager::GetInstance()->InitializeAudio();
    FrustumCulling::GetInstance()->UpdateFrustum();

    game = std::make_shared<Game>();
    game->InitializeGame();

    lastFrameTime = (float)glfwGetTime();
    lastFixedFrameTime = (float)glfwGetTime();
    lastAIFrameTime = (float)glfwGetTime();
}

void GloomEngine::Awake() {
    for (auto&& component : components) {
        if (component.second->callOnAwake) component.second->Awake();
    }
}

void GloomEngine::Start() {
    for (auto&& component : components){
        if (component.second->enabled && component.second->callOnStart) component.second->Start();
    }

    // Load game
    std::filesystem::path path = std::filesystem::current_path();
    DataPersistanceManager::GetInstance()->LoadGame(path.string(), "Save1");
}

bool GloomEngine::MainLoop() {
    auto currentTime = (float)glfwGetTime();

    glfwPollEvents();
    glfwSetWindowSize(window, width, height);

    int multiplier60Rate = (int)((currentTime - (float)(int)currentTime) * 60);
    int multiplier60LastRate = (int)((lastFrameTime - (float)(int)lastFrameTime) * 60);
    if (multiplier60Rate > multiplier60LastRate || (multiplier60Rate == 0 && multiplier60LastRate != 0)) {
        glClearColor(screenColor.x, screenColor.y, screenColor.z, screenColor.w);

        Update();

        deltaTime = (currentTime - lastFrameTime) * timeScale;
        if (deltaTime > idealDeltaTime + 0.01f) deltaTime = idealDeltaTime;
        lastFrameTime = currentTime;

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    int multiplier120Rate = (int)((currentTime - (float)(int)currentTime) * 120);
    int multiplier120LastRate = (int)((lastFixedFrameTime - (float)(int)lastFixedFrameTime) * 120);
    if (multiplier120Rate > multiplier120LastRate || (multiplier120Rate == 0 && multiplier120LastRate != 0)) {

        if (timeScale != 0) {
            FixedUpdate();
        }

        fixedDeltaTime = (currentTime - lastFixedFrameTime) * timeScale;
        if (fixedDeltaTime > idealFixedDeltaTime + 0.01f) fixedDeltaTime = idealFixedDeltaTime;
        lastFixedFrameTime = currentTime;
    }

    int multiplier2Rate = (int)((currentTime - (float)(int)currentTime) * 2);
    int multiplier2LastRate = (int)((lastAIFrameTime - (float)(int)lastAIFrameTime) * 2);
    if (multiplier2Rate > multiplier2LastRate || (multiplier2Rate == 0 && multiplier2LastRate != 0)) {

        if (timeScale != 0) {
            AIUpdate();
        }

        AIDeltaTime = (currentTime - lastAIFrameTime) * timeScale;
        if (AIDeltaTime > idealAIDeltaTime + 0.01f) AIDeltaTime = idealAIDeltaTime;
        lastAIFrameTime = currentTime;
    }

#ifdef DEBUG
    engineDeltaTime = (currentTime - lastEngineDeltaTime);
    lastEngineDeltaTime = currentTime;
#endif

    bool endGame = game->GameLoop();

//  Save game on quit
    if (glfwWindowShouldClose(window) || endGame) {
        std::filesystem::path path = std::filesystem::current_path();
        DataPersistanceManager::GetInstance()->SaveGame(path.string(), "Save1");
    }
    return glfwWindowShouldClose(window) || endGame;
}

void GloomEngine::Update() {
    FrustumCulling::GetInstance()->UpdateFrustum();

    for (auto&& component : components) {
        if (component.second->callOnAwake) component.second->Awake();
        if (component.second->callOnStart && component.second->enabled) component.second->Start();
        if (component.second->enabled) component.second->Update();
    }

    // Prepare shadow framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, ShadowManager::GetInstance()->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ShadowManager::GetInstance()->PrepareShadow();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, width, height);

    // Prepare texture framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, PostProcessingManager::GetInstance()->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D,  ShadowManager::GetInstance()->depthMap);
    RendererManager::GetInstance()->DrawObjects();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    PostProcessingManager::GetInstance()->DrawBuffer();

    glEnable(GL_DEPTH_TEST);

#ifdef DEBUG
    CollisionManager::GetInstance()->DrawColliders();
#endif

    UIManager::GetInstance()->DrawUI();

#ifdef DEBUG
    DebugManager::GetInstance()->Render();
#endif

    HIDManager::GetInstance()->ManageInput();
}

void GloomEngine::FixedUpdate() {
    for (auto&& component : components) {
        if (component.second->enabled) component.second->FixedUpdate();
    }

    CollisionManager::GetInstance()->ManageCollision();
}

void GloomEngine::AIUpdate() {
    for (auto&& component : components) {
        if (component.second->enabled) component.second->AIUpdate();
    }
}

void GloomEngine::Free() const {
    CollisionManager::GetInstance()->Free();
    AudioManager::GetInstance()->Free();
    ShadowManager::GetInstance()->Free();
    RendererManager::GetInstance()->Free();
    PostProcessingManager::GetInstance()->Free();
    UIManager::GetInstance()->Free();
#ifdef DEBUG
    DebugManager::GetInstance()->Free();
#endif
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
    glfwWindowHint(GLFW_SAMPLES, 4);
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
    glEnable(GL_MULTISAMPLE);
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
