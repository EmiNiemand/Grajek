#include "EngineManagers/SceneManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Camera.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Interfaces/SaveableStaticObject.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#include <fstream>

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
}

void SceneManager::Free() {
    ClearScene();
    Camera::activeCamera = nullptr;
    activeScene = nullptr;
}

void SceneManager::SaveStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::vector<std::shared_ptr<SaveableStaticObject>> saveableStaticPrefabs;
    std::vector<std::shared_ptr<StaticObjectData>> staticObjectsData;
    saveableStaticPrefabs = FindAllStaticSaveablePrefabs();
    for (const auto& object : saveableStaticPrefabs) {
        staticObjectsData.push_back(object->SaveStatic());
    }

    SaveMap(staticObjectsData,dataDirectoryPath,dataFileName);
}

void SceneManager::LoadStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::vector<std::shared_ptr<StaticObjectData>> staticObjectsData;
    staticObjectsData = LoadMap(dataDirectoryPath, dataFileName);

    for (const auto &object: staticObjectsData) {
        //Create new prefabs using data from the file
        //GameObject newchild = GameObject();
        //activeScene->AddChild();
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
        objectJson["position.x"] = object->position.x;
        objectJson["position.y"] = object->position.y;
        objectJson["position.z"] = object->position.z;

        objectJson["rotation.x"] = object->position.x;
        objectJson["rotation.y"] = object->position.y;
        objectJson["rotation.z"] = object->position.z;

        objectJson["scale.x"] = object->position.x;
        objectJson["scale.y"] = object->position.y;
        objectJson["scale.z"] = object->position.z;

        json.emplace(objectJson);
    }

}

void SceneManager::from_json(const nlohmann::json &json, std::vector<std::shared_ptr<StaticObjectData>>& mapData) {
    mapData.clear();
    std::shared_ptr<StaticObjectData> newObject;
    for (const auto& object: json){
        newObject = std::make_shared<StaticObjectData>();
        newObject->position.x = object["position.x"];
        newObject->position.y = object["position.y"];
        newObject->position.z = object["position.z"];

        newObject->rotation.x = object["rotation.x"];
        newObject->rotation.y = object["rotation.y"];
        newObject->rotation.z = object["rotation.z"];

        newObject->scale.x = object["position.x"];
        newObject->scale.y = object["position.y"];
        newObject->scale.z = object["position.z"];
        mapData.push_back(newObject);
    }
}

std::vector<std::shared_ptr<SaveableStaticObject>> SceneManager::FindAllStaticSaveablePrefabs() {
    std::vector<std::shared_ptr<SaveableStaticObject>> objects;

    for (const auto& object : activeScene->children) {
        if (std::dynamic_pointer_cast<SaveableStaticObject>(object.second) != nullptr) {
            objects.push_back(std::dynamic_pointer_cast<SaveableStaticObject>(object.second));
        }
    }
    return objects;
}