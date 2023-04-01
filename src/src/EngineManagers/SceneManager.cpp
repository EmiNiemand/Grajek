#include "EngineManagers/SceneManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Camera.h"
#include "GameObjectsAndPrefabs/GameObject.h"

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() = default;

SceneManager* SceneManager::GetInstance() {
    if (sceneManager == nullptr) {
        sceneManager = new SceneManager();
    }
    return sceneManager;
}

void SceneManager::InitializeScene() {
    activeScene = GameObject::Instantiate("Scene", nullptr, Tags::SCENE);
    Camera::activeCamera = GameObject::Instantiate("Camera", activeScene, Tags::CAMERA);
}

void SceneManager::ClearScene() {
    activeScene->RemoveAllChildren();
    GloomEngine::GetInstance()->components.clear();
    GloomEngine::GetInstance()->gameObjects.clear();
}

void SceneManager::Free() {
    ClearScene();
    Camera::activeCamera = nullptr;
    activeScene = nullptr;
}

