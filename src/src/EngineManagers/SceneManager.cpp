#include "EngineManagers/SceneManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Camera.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Interfaces/SaveableStaticObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "GameObjectsAndPrefabs/Prefabs/House.h"
#include "GameObjectsAndPrefabs/Prefabs/Shop.h"
#include "GameObjectsAndPrefabs/Prefabs/SavePoint.h"
#include "GameObjectsAndPrefabs/Prefabs/MainMenuPrefab.h"
#include "Game.h"
#include "Components/Renderers/Animator.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Menus/LoadGameMenu.h"

#include <fstream>


#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() {
    delete sceneManager;
}

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

    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "ProjectConfig";

    SceneManager::GetInstance()->LoadStaticObjects(path.string(),"map0");
}

void SceneManager::LoadScene(const std::string& scene) {
    if (scene == "Scene") {
        file = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGameMenu")->GetComponent<LoadGameMenu>()->file;
        ClearScene();
        InitializeScene();
        GloomEngine::GetInstance()->game->activeCamera = Camera::activeCamera;
        GloomEngine::GetInstance()->game->activeScene = activeScene;
        GloomEngine::GetInstance()->game->InitializeGame();
    } else if (scene == "MainMenu") {
        ClearScene();
        activeScene = GameObject::Instantiate("MainMenuScene", nullptr, Tags::SCENE);
        Prefab::Instantiate<MainMenuPrefab>();
    }
}

void SceneManager::ClearScene() const {
    activeScene->RemoveAllChildren();
    Animator::animationModels.clear();
    Animator::animations.clear();
    Renderer::models.clear();
    GameObject::Destroy(activeScene);
}

void SceneManager::Free() {
    ClearScene();
    Camera::activeCamera = nullptr;
    activeScene = nullptr;
}

void SceneManager::SaveStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::map<int,std::shared_ptr<SaveableStaticObject>> saveableStaticPrefabs;
    std::vector<std::shared_ptr<StaticObjectData>> staticObjectsData;
    std::shared_ptr<StaticObjectData> newObject;

    saveableStaticPrefabs = FindAllStaticSaveablePrefabs();
    for (const auto& object : saveableStaticPrefabs) {
        newObject = object.second->SaveStatic();
        staticObjectsData.push_back(newObject);
    }

    SaveMap(staticObjectsData,dataDirectoryPath,dataFileName);
}

void SceneManager::LoadStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::vector<std::shared_ptr<StaticObjectData>> staticObjectsData;
    staticObjectsData = LoadMap(dataDirectoryPath, dataFileName);
    spdlog::info("Loaded map objects successfully.");

    std::shared_ptr<GameObject> newGameObject;
    for (const auto &object: staticObjectsData) {
        newGameObject.reset();
        if(object->name == "House"){
            newGameObject = Prefab::Instantiate<House>();
        }
        if(object->name == "Shop"){
            newGameObject = Prefab::Instantiate<Shop>();
        }
        if(object->name == "SavePoint"){
            newGameObject = Prefab::Instantiate<SavePoint>();
        }
        if(newGameObject) {
            newGameObject->transform->SetLocalPosition(object->position);
            newGameObject->transform->SetLocalRotation(object->rotation);
            newGameObject->transform->SetLocalScale(object->scale);

            if (newGameObject->GetComponent<Renderer>()) {
                std::shared_ptr<Renderer> objectRenderer = newGameObject->GetComponent<Renderer>();
                objectRenderer->LoadModel(object->modelPath);
            }

            if (newGameObject->GetComponent<BoxCollider>()) {
                std::shared_ptr<BoxCollider> objectColider = newGameObject->GetComponent<BoxCollider>();
                objectColider->SetSize(object->coliderSize);
                objectColider->SetOffset(object->coliderOffset);
            }

        }
    }
}

std::vector<std::shared_ptr<StaticObjectData>> SceneManager::LoadMap(std::string dataDirectoryPath, std::string dataFileName) {
    std::filesystem::path path(dataDirectoryPath);
    path /= dataFileName + ".json";
#ifdef DEBUG
    spdlog::info("Save path: " + path.string());
#endif

    std::vector<std::shared_ptr<StaticObjectData>> mapData = {};

    try {
        std::ifstream input(path);

        nlohmann::json json;
        input >> json;
        from_json(json, mapData);
    }
    catch (std::exception e) {
        spdlog::info("Failed to read a file content at path: " + path.string());
    }

    return mapData;
}

void SceneManager::SaveMap(std::vector<std::shared_ptr<StaticObjectData>> mapData, std::string dataDirectoryPath, std::string dataFileName) {
    std::filesystem::path path(dataDirectoryPath);
    path /= dataFileName + ".json";

#ifdef DEBUG
    spdlog::info("Save path: " + path.string());
#endif

    try {
        nlohmann::json json;
        to_json(json, mapData);

        std::ofstream saveFile(path);
        saveFile << json << std::endl;
    }
    catch(std::exception e) {
        spdlog::info("Failed to write a MAP file content at path: " + path.string());
    }
}

void SceneManager::to_json(nlohmann::json &json, std::vector<std::shared_ptr<StaticObjectData>>& mapData) {
    json = nlohmann::json::array({});
    nlohmann::json objectJson;
    for (const auto& object: mapData){
        objectJson.clear();

        objectJson["name"] = object -> name;

        objectJson["position.x"] = object->position.x;
        objectJson["position.y"] = object->position.y;
        objectJson["position.z"] = object->position.z;

        objectJson["rotation.x"] = object->rotation.x;
        objectJson["rotation.y"] = object->rotation.y;
        objectJson["rotation.z"] = object->rotation.z;

        objectJson["scale.x"] = object->scale.x;
        objectJson["scale.y"] = object->scale.y;
        objectJson["scale.z"] = object->scale.z;

        objectJson["modelPath"] = object->modelPath;

        objectJson["coliderSize.x"] = object->coliderSize.x;
        objectJson["coliderSize.y"] = object->coliderSize.y;
        objectJson["coliderSize.z"] = object->coliderSize.z;

        objectJson["coliderOffset.x"] = object->coliderOffset.x;
        objectJson["coliderOffset.y"] = object->coliderOffset.y;
        objectJson["coliderOffset.z"] = object->coliderOffset.z;

        json.push_back(objectJson);
    }

}

void SceneManager::from_json(const nlohmann::json &json, std::vector<std::shared_ptr<StaticObjectData>>& mapData) {
    mapData.clear();
    std::shared_ptr<StaticObjectData> newObject;
    for (const auto& object: json){
        newObject = std::make_shared<StaticObjectData>();

        newObject->name = object["name"];

        newObject->position.x = object["position.x"];
        newObject->position.y = object["position.y"];
        newObject->position.z = object["position.z"];

        newObject->rotation.x = object["rotation.x"];
        newObject->rotation.y = object["rotation.y"];
        newObject->rotation.z = object["rotation.z"];

        newObject->scale.x = object["scale.x"];
        newObject->scale.y = object["scale.y"];
        newObject->scale.z = object["scale.z"];

        if(object.contains("modelPath"))
        newObject->modelPath = object["modelPath"];
        if(object.contains("coliderSize.x"))
        newObject->coliderSize.x = object["coliderSize.x"];
        if(object.contains("coliderSize.y"))
        newObject->coliderSize.y = object["coliderSize.y"];
        if(object.contains("coliderSize.z"))
        newObject->coliderSize.z = object["coliderSize.z"];

        if(object.contains("coliderOffset.x"))
        newObject->coliderOffset.x = object["coliderOffset.x"];
        if(object.contains("coliderOffset.y"))
        newObject->coliderOffset.y = object["coliderOffset.y"];
        if(object.contains("coliderOffset.z"))
        newObject->coliderOffset.z = object["coliderOffset.z"];
        
        mapData.push_back(newObject);
    }
}

std::map<int, std::shared_ptr<SaveableStaticObject>> SceneManager::FindAllStaticSaveablePrefabs() {
    std::map<int,std::shared_ptr<SaveableStaticObject>> objects;
    int i = 0;
    for (const auto& object : SceneManager::GetInstance()->activeScene->children) {
        i++;
        if (std::dynamic_pointer_cast<SaveableStaticObject>(object.second) != nullptr) {
            objects[object.first] = (std::dynamic_pointer_cast<SaveableStaticObject>(object.second));
        }
    }

    return objects;
}

void SceneManager::CreatePrefabObject(const std::string name) {
    if(name == "House"){
        spdlog::info("Created object from prefab House.");
        Prefab::Instantiate<House>();
    } else if(name == "Shop"){
        spdlog::info("Created object from prefab Shop");
        Prefab::Instantiate<Shop>();
    } else if(name == "SavePoint"){
        spdlog::info("Created object from prefab SavePoint");
        Prefab::Instantiate<SavePoint>();
    } else {
        spdlog::info("Failed to find prefab with name: " + name);
    }
}
void SceneManager::CreatePrefabObject(const std::string name,const std::string modelPath) {
    std::shared_ptr<GameObject> newGameObject;
    if(name == "House"){
        spdlog::info("Created object from prefab House. Using model: " + modelPath);
        newGameObject = Prefab::Instantiate<House>();
        std::shared_ptr<Renderer> objectRenderer = newGameObject->GetComponent<Renderer>();
        objectRenderer->LoadModel(modelPath);
    } else {
        spdlog::info("Failed to find prefab with name: " + name);
    }
}
