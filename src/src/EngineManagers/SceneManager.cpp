#include "include/EngineManagers/SceneManager.h"
#include "include/GloomEngine.h"
#include "include/Components/Renderers/Camera.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"

SceneManager::SceneManager(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

SceneManager::~SceneManager() {}

void SceneManager::InitializeScene() {
    activeScene = GameObject::Instantiate("Scene", nullptr, Tags::SCENE);
    Camera::activeCamera = GameObject::Instantiate("Camera", activeScene, Tags::CAMERA);
}

void SceneManager::ClearScene() {
    activeScene->RemoveAllChildren();
    gloomEngine->components.clear();
    gloomEngine->gameObjects.clear();
}

void SceneManager::Free() {
    ClearScene();
    Camera::activeCamera = nullptr;
    activeScene = nullptr;
}


