#ifndef OPENGLGP_GLOOMENGINE_H
#define OPENGLGP_GLOOMENGINE_H

#pragma region "Library includes"
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
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

class Component;
class GameObject;
class GameObjectFactory;
class ComponentFactory;
class EngineRenderer;
class EngineColliders;

class GloomEngine : public std::enable_shared_from_this<GloomEngine> {
private:
    std::map<int, std::shared_ptr<GameObject>> gameObjects;
    std::map<int, std::shared_ptr<Component>> components;

    std::unique_ptr<GameObjectFactory> gameObjectFactory;
    std::unique_ptr<ComponentFactory> componentFactory;

public:
    GLFWwindow* window;
    int* width;
    int* height;

    std::unique_ptr<EngineRenderer> engineRenderer;
    std::unique_ptr<EngineColliders> engineColliders;

    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    std::shared_ptr<GameObject> activeCamera;
    std::shared_ptr<GameObject> activeScene;

public:
    GloomEngine(GLFWwindow* window, int *width, int *height);
    virtual ~GloomEngine();

    /// Init creates all needed variables like factories
    void Init();
    /// Awake calls all the components even when they are no enabled during first frame
    void Awake();
    /// Start calls only enabled components during first frame
    void Start();
    /// Return false if game should not end
    /// Return true to end the update loop and end the game
    /// Update only enabled components
    bool Update();
    /// Free memory
    void Destroy();

    void AddGameObject(std::shared_ptr<GameObject> gameObject);
    void AddComponent(std::shared_ptr<Component> component);

    void OnUpdate(int componentId);

    void RemoveGameObject(std::shared_ptr<GameObject> gameObject);
    void RemoveComponent(std::shared_ptr<Component> component);

    std::shared_ptr<GameObject> FindGameObjectWithId(int id);
    std::shared_ptr<GameObject> FindGameObjectWithName(std::string name);

private:
    void ClearScene();
};


#endif //OPENGLGP_GLOOMENGINE_H
