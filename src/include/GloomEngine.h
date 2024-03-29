#ifndef OPENGLGP_GLOOMENGINE_H
#define OPENGLGP_GLOOMENGINE_H

#pragma region "Library includes"
#ifdef DEBUG
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <cstdio>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>
#pragma endregion

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ProjectSettings.h"
#include <map>
#include <string>

class GameObjectFactory;
class ComponentFactory;
class RendererManager;
class CollisionManager;
class HIDManager;
class SceneManager;
class DataPersistanceManager;

class Game;
class GameObject;
class Component;


class GloomEngine {
private:
    friend class SceneManager;
    friend class DataPersistanceManager;
    friend class GameObject;
    friend class Transform;

    /// Do not touch this variable
    inline static GloomEngine* gloomEngine;

    std::map<int, std::shared_ptr<GameObject>> gameObjects = {};
    std::map<int, std::shared_ptr<Component>> components = {};
    std::map<int, std::shared_ptr<Component>> componentsCopy = {};

public:
    GLFWwindow* window;

    glm::vec4 screenColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);

    unsigned int destroyGameObjectBufferIterator = 0;
    std::shared_ptr<GameObject> destroyGameObjectBuffer[2000];
    unsigned int destroyComponentBufferIterator = 0;
    std::shared_ptr<Component> destroyComponentBuffer[2000];

    std::shared_ptr<Game> game;
    /// set to 0 to pause, 1 to resume
    float timeScale = 1;

#ifdef DEBUG
    // Timers for Engine
    float engineDeltaTime = 0.0f;
    float lastEngineDeltaTime = 0.0f;

    const char* const mainLoop = "MainLoop";
#endif

    // Timers for update
    float idealDeltaTime = 1.0f / 60;
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    // Timers for fixedUpdate
    float idealFixedDeltaTime = 1.0f / 120;
    float fixedDeltaTime = 0.0f;
    float lastFixedFrameTime = 0.0f;

    // Timers for AI update
    float idealAIDeltaTime = 1.0f / 2;
    float AIDeltaTime = 0.0f;
    float lastAIFrameTime = 0.0f;

public:
    GloomEngine(GloomEngine &other) = delete;
    void operator=(const GloomEngine&) = delete;
    virtual ~GloomEngine();

    static GloomEngine* GetInstance();

    /// Initialize creates all needed variables like factories
    void Initialize();
    /// Return false if game should not end
    /// Return true to end the update loop and end the game
    bool MainLoop();
    /// Updates components with 60Hz rate
    void Update();
    /// Updates components with 120Hz rate
    void FixedUpdate();
    /// Updates components with 2Hz rate
    void AIUpdate();
    /// Free memory
    void Free() const;

    std::shared_ptr<GameObject> FindGameObjectWithId(int id);
    std::shared_ptr<GameObject> FindGameObjectWithName(const std::string& name);

    void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
    void AddComponent(const std::shared_ptr<Component>& component);

    void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
    void RemoveComponent(const std::shared_ptr<Component>& component);

    void AddGameObjectToDestroyBuffer(const std::shared_ptr<GameObject>& gameObject);
    void AddComponentToDestroyBuffer(const std::shared_ptr<Component>& component);

private:
    GloomEngine();
    void InitializeWindow();
    static void glfwErrorCallback(int error, const char* description);
};


#endif //OPENGLGP_GLOOMENGINE_H
