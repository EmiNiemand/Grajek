#ifndef OPENGLGP_GLOOMENGINE_H
#define OPENGLGP_GLOOMENGINE_H

#pragma region "Library includes"
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
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
class ColliderManager;
class HIDManager;
class SceneManager;
class DataPersistanceManager;

class Game;
class GameObject;
class Component;


class GloomEngine : public std::enable_shared_from_this<GloomEngine> {
private:
    friend class GameObject;
    friend class GameObjectFactory;
    friend class ComponentFactory;
    friend class RendererManager;
    friend class ColliderManager;
    friend class HIDManager;
    friend class SceneManager;
    friend class DataPersistanceManager;

    std::map<int, std::shared_ptr<GameObject>> gameObjects = {};
    std::map<int, std::shared_ptr<Component>> components = {};

public:
    GLFWwindow* window;
    int32_t width;
    int32_t height;

    glm::vec4 screenColor = glm::vec4(0.10f, 0.10f, 0.10f, 1.00f);

    std::unique_ptr<RendererManager> rendererManager;
    std::unique_ptr<ColliderManager> colliderManager;
    std::unique_ptr<HIDManager> hidManager;
    std::unique_ptr<SceneManager> sceneManager;

    std::shared_ptr<Game> game;

    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

public:
    GloomEngine();
    virtual ~GloomEngine();

    /// Initialize creates all needed variables like factories
    void Initialize();
    /// Awake calls all the components even when they are no enabled during first frame
    void Awake();
    /// Start calls only enabled components during first frame
    void Start();
    /// Return false if game should not end
    /// Return true to end the update loop and end the game
    /// Update only enabled components
    bool Update();
    /// Free memory
    void Free();

    std::shared_ptr<GameObject> FindGameObjectWithId(int id);
    std::shared_ptr<GameObject> FindGameObjectWithName(std::string name);

private:
    void InitializeWindow();
    static void glfwErrorCallback(int error, const char* description);

    void AddGameObject(std::shared_ptr<GameObject> gameObject);
    void AddComponent(std::shared_ptr<Component> component);

    void RemoveGameObject(std::shared_ptr<GameObject> gameObject);
    void RemoveComponent(std::shared_ptr<Component> component);
};


#endif //OPENGLGP_GLOOMENGINE_H
