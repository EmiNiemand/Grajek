#include "GloomEngine.h"
#include "Game.h"
#include "EngineManagers/AudioManager.h"
#include "EngineManagers/RendererManager.h"
#include "EngineManagers/PostProcessingManager.h"
#include "EngineManagers/UIManager.h"
#include "EngineManagers/CollisionManager.h"
#include "EngineManagers/ShadowManager.h"
#include "EngineManagers/AnimationManager.h"
#include "EngineManagers/HIDManager.h"
#include "EngineManagers/SceneManager.h"
#include "EngineManagers/DataPersistanceManager.h"
#include "EngineManagers/OptionsManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "EngineManagers/AIManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/Camera.h"
#include "Components/Renderers/Lights/PointLight.h"
#include "Components/Renderers/Lights/DirectionalLight.h"
#include "Components/Renderers/Lights/SpotLight.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerMovement.h"
#include "Other/FrustumCulling.h"
#include "Components/Renderers/Animator.h"

#include <filesystem>
#include <stb_image.h>

#ifdef DEBUG
#include "EngineManagers/DebugManager.h"
#include <tracy/Tracy.hpp>
#endif

GloomEngine::GloomEngine() {}

GloomEngine::~GloomEngine() = default;

GloomEngine* GloomEngine::GetInstance() {
    if (gloomEngine == nullptr) {
        gloomEngine = new GloomEngine();
    }
    return gloomEngine;
}

void GloomEngine::Initialize() {
#ifdef DEBUG
    ZoneScopedNC("Init", 0xDC143C);
#endif
    OptionsManager::GetInstance()->Load();
    InitializeWindow();

    SceneManager::GetInstance()->InitializeScene();
    RendererManager::GetInstance()->UpdateProjection();
    AudioManager::GetInstance()->InitializeAudio();
    RandomnessManager::GetInstance()->InitializeRandomEngine();
    AIManager::GetInstance()->InitializeSpawner(5, 10, 100);

    game = std::make_shared<Game>();
    game->InitializeGame();

    lastFrameTime = (float)glfwGetTime();
    lastFixedFrameTime = (float)glfwGetTime();
    lastAIFrameTime = (float)glfwGetTime();
}

bool GloomEngine::MainLoop() {
#ifdef DEBUG
    FrameMarkStart(mainLoop);
#endif
    auto currentTime = (float)glfwGetTime();

    glfwPollEvents();
    glfwSetWindowSize(window, OptionsManager::GetInstance()->width, OptionsManager::GetInstance()->height);

    // AI UPDATE
    int multiplier2Rate = (int)((currentTime - (float)(int)currentTime) * 2);
    int multiplier2LastRate = (int)((lastAIFrameTime - (float)(int)lastAIFrameTime) * 2);
    // FIXED UPDATE
    int multiplier120Rate = (int)((currentTime - (float)(int)currentTime) * 120);
    int multiplier120LastRate = (int)((lastFixedFrameTime - (float)(int)lastFixedFrameTime) * 120);
    // UPDATE
    int multiplier60Rate = (int)((currentTime - (float)(int)currentTime) * 60);
    int multiplier60LastRate = (int)((lastFrameTime - (float)(int)lastFrameTime) * 60);


    if (multiplier120Rate > multiplier120LastRate || (multiplier120Rate == 0 && multiplier120LastRate != 0)) {
        for (int i = 0; i < destroyComponentBufferIterator; ++i) {
            const auto& component = destroyComponentBuffer[i];
            component->OnDestroy();
            component->GetParent()->RemoveComponent((int)component->GetId());
            RemoveComponent(component);
        }
        ClearDestroyComponentBuffer();

        for (int i = 0; i < destroyGameObjectBufferIterator; ++i) {
            const auto& gameObject = destroyGameObjectBuffer[i];
            gameObject->parent->RemoveChild(gameObject->GetId());
            RemoveGameObject(gameObject);
        }
        ClearDestroyGameObjectBuffer();

        SceneManager::GetInstance()->activeScene->UpdateSelfAndChildren();

        componentsCopy = components;

        for (const auto& component: componentsCopy) {
            if (component.second->callOnAwake) {
                component.second->Awake();
                component.second->GetParent()->UpdateSelfAndChildren();
            }
            if (component.second->callOnStart && component.second->enabled) {
                component.second->Start();
                component.second->GetParent()->UpdateSelfAndChildren();
            }
        }
    }

    // AI UPDATE
    if (multiplier2Rate > multiplier2LastRate || (multiplier2Rate == 0 && multiplier2LastRate != 0)) {
#ifdef DEBUG
        ZoneScopedNC("AI update", 0x00FF00);
#endif
        if (timeScale != 0) {
            AIUpdate();
        }

        AIDeltaTime = (currentTime - lastAIFrameTime) * timeScale;
        if (AIDeltaTime > idealAIDeltaTime + 0.01f) AIDeltaTime = idealAIDeltaTime;
        lastAIFrameTime = currentTime;
    }

    // FIXED UPDATE
    if (multiplier120Rate > multiplier120LastRate || (multiplier120Rate == 0 && multiplier120LastRate != 0)) {
#ifdef DEBUG
        ZoneScopedNC("Fixed update", 0x00008B);
#endif
        if (timeScale != 0) {
            FixedUpdate();
        }

        fixedDeltaTime = (currentTime - lastFixedFrameTime) * timeScale;
        if (fixedDeltaTime > idealFixedDeltaTime + 0.01f) fixedDeltaTime = idealFixedDeltaTime;
        lastFixedFrameTime = currentTime;
    }

    // UPDATE
    if (multiplier60Rate > multiplier60LastRate || (multiplier60Rate == 0 && multiplier60LastRate != 0)) {
#ifdef DEBUG
        ZoneScopedNC("Update", 0xDC143C);
#endif
        glClearColor(screenColor.x, screenColor.y, screenColor.z, screenColor.w);

        Update();

        deltaTime = (currentTime - lastFrameTime) * timeScale;
        if (deltaTime > idealDeltaTime + 0.01f) deltaTime = idealDeltaTime;
        lastFrameTime = currentTime;

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

#ifdef DEBUG
    engineDeltaTime = (currentTime - lastEngineDeltaTime);
    lastEngineDeltaTime = currentTime;
    FrameMark;
#endif

    bool endGame = game->GameLoop();

    return glfwWindowShouldClose(window) || endGame;
}

void GloomEngine::Update() {
    //Frustum culling
    {
#ifdef DEBUG
        ZoneScopedNC("Frustum Culling", 0xFFD733);
#endif
        FrustumCulling::GetInstance()->UpdateFrustum();

        for (const auto& gameObject: gameObjects) {

            gameObject.second->isOnFrustum = FrustumCulling::GetInstance()->IsOnFrustum(gameObject.second->bounds,
                                                                                        gameObject.second->transform);
        }
    }
    // Component update
    {
#ifdef DEBUG
        ZoneScopedNC("Component update", 0xFF69B4);
#endif
        for (const auto& component: componentsCopy) {
            if (component.second->enabled) {
                component.second->Update();
            }
        }
        AnimationManager::GetInstance()->UpdateAnimations();

    }
    // Preparing shadow map
    if (!FindGameObjectWithName("MainMenu"))
    {
#ifdef DEBUG
        ZoneScopedNC("Prepare shadow", 0xFFD733);
#endif

        // Prepare shadow framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowManager::GetInstance()->depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        ShadowManager::GetInstance()->PrepareShadow();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    // Drawing objects to textures
    {
#ifdef DEBUG
        ZoneScopedNC("Draw objects", 0xADD8E6);
#endif
        glViewport(0, 0, OptionsManager::GetInstance()->width, OptionsManager::GetInstance()->height);

        // Prepare texture framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, PostProcessingManager::GetInstance()->framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, ShadowManager::GetInstance()->depthMap);
        RendererManager::GetInstance()->Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    // Applying post-processing
    {
#ifdef DEBUG
        ZoneScopedNC("Post processing", 0xFFD733);
#endif
        PostProcessingManager::GetInstance()->Draw();

        glEnable(GL_DEPTH_TEST);
    }
    // Drawing debug lines for colliders
    if (!FindGameObjectWithName("MainMenu"))
    {
#ifdef DEBUG
        ZoneScopedNC("Draw colliders", 0x800000);
        CollisionManager::GetInstance()->Draw();
#endif
    }
    // Drawing UI elements
    {
#ifdef DEBUG
        ZoneScopedNC("Draw UI", 0xFFD733);
#endif

        UIManager::GetInstance()->Draw();
    }
    // Rendering IMGUI debug windows
    {
#ifdef DEBUG
        ZoneScopedNC("Debug windows", 0xC71585);
        DebugManager::GetInstance()->Render();
#endif
    }
    // Managing input
    {
#ifdef DEBUG
        ZoneScopedNC("Manage input", 0x800080);
#endif
        HIDManager::GetInstance()->ManageInput();
    }
}

void GloomEngine::FixedUpdate() {
    for (const auto& component : componentsCopy) {
        if (component.second->enabled) component.second->FixedUpdate();
    }

    CollisionManager::GetInstance()->ManageCollision();
}

void GloomEngine::AIUpdate() {
    for (const auto& component : componentsCopy) {
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
    RandomnessManager::GetInstance()->Free();
    AIManager::GetInstance()->Free();
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
#ifdef DEBUG
    ZoneScopedNC("Window Init", 0xDC143C);
#endif
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
    window = glfwCreateWindow(OptionsManager::GetInstance()->width, OptionsManager::GetInstance()->height, "Gloomies", NULL, NULL);
    if (window == nullptr)
        throw;
    glfwMakeContextCurrent(window);
    // Enable vsync
    glfwSwapInterval(false);

    // Center window on the screen
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int monitorWidth = mode->width;
    int monitorHeight = mode->height;
    glfwSetWindowPos(window, monitorWidth / 2 - OptionsManager::GetInstance()->width / 2, monitorHeight / 2 - OptionsManager::GetInstance()->height / 2);

#ifdef RELEASE
    // Enable cursor - change last parameter to disable it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

#ifdef DEBUG
    // Enable cursor - change last parameter to disable it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif

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
    stbi_set_flip_vertically_on_load(true);
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

void GloomEngine::AddGameObjectToDestroyBuffer(const std::shared_ptr<GameObject>& gameObject) {
    destroyGameObjectBuffer[destroyGameObjectBufferIterator] = gameObject;
    ++destroyGameObjectBufferIterator;
}

void GloomEngine::AddComponentToDestroyBuffer(const std::shared_ptr<Component>& component) {
    destroyComponentBuffer[destroyComponentBufferIterator] = component;
    ++destroyComponentBufferIterator;
}

void GloomEngine::ClearDestroyGameObjectBuffer() {
    for (int i = 0; i < destroyGameObjectBufferIterator; ++i) {
        destroyGameObjectBuffer[i] = nullptr;
    }
    destroyGameObjectBufferIterator = 0;
}

void GloomEngine::ClearDestroyComponentBuffer() {
    for (int i = 0; i < destroyComponentBufferIterator; ++i) {
        destroyComponentBuffer[i] = nullptr;
    }
    destroyComponentBufferIterator = 0;
}
