#include "EngineManagers/SceneManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Camera.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "LowLevelClasses/StaticObjData.h"
#include "LowLevelClasses/FileDataHandler.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() = default;

SceneManager* SceneManager::GetInstance() {
    if (sceneManager == nullptr) {
        sceneManager = new SceneManager();
    }
    return sceneManager;
}

void SceneManager::InitializeScene() {
#ifdef DEBUG
    ZoneScopedNC("Scene Init", 0xDC143C);
#endif
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

void SceneManager::SaveStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::vector<std::shared_ptr<StaticObjData>> staticObjectsData;
    for (const auto& object : activeScene->children) {
        object.second->SaveStatic();
    }

    FileDataHandler fileDataHandler(dataDirectoryPath, dataFileName);
    fileDataHandler.SaveMap(staticObjectsData);
}
//#ifdef DEBUG
//    spdlog::info("Saving game: " + std::to_string(gameData->money) + ", " + std::to_string(gameData->reputation)
//                 + ", [" + std::to_string(gameData->playerPosition.x) + ", " + std::to_string(gameData->playerPosition.y)
//                 + ", " + std::to_string(gameData->playerPosition.z) + "]");
//#endif

void SceneManager::LoadStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::vector<std::shared_ptr<StaticObjData>> staticObjectsData;
    FileDataHandler fileDataHandler(dataDirectoryPath, dataFileName);
    staticObjectsData = fileDataHandler.LoadMap();

    std::vector<std::shared_ptr<IStaticSaveable>> dataPersistanceObjects = FindAllStaticSaveableObjects();

    for (const auto &object: dataPersistanceObjects) {
        //GameObject newchild = GameObject(object);
        //activeScene->AddChild();
    }
}