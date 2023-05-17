#include "EngineManagers/SceneManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Camera.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Interfaces/SaveableStaticObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "GameObjectsAndPrefabs/Prefabs/House.h"

#include <fstream>

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

    std::filesystem::path path = std::filesystem::current_path().parent_path().parent_path();
    path /= "res";
    path /= "ProjectConfig";

    SceneManager::GetInstance()->LoadStaticObjects(path.string(),"map0");
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
    std::map<int,std::shared_ptr<SaveableStaticObject>> saveableStaticPrefabs;
    std::vector<std::shared_ptr<StaticObjectData>> staticObjectsData;
    saveableStaticPrefabs = FindAllStaticSaveablePrefabs();
    for (const auto& object : saveableStaticPrefabs) {
        staticObjectsData.push_back(object.second->SaveStatic());
    }

    SaveMap(staticObjectsData,dataDirectoryPath,dataFileName);
}

void SceneManager::LoadStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName) {
    std::vector<std::shared_ptr<StaticObjectData>> staticObjectsData;
    staticObjectsData = LoadMap(dataDirectoryPath, dataFileName);
    spdlog::info("Loaded map objects successfully.");

    std::shared_ptr<GameObject> newGameObject;
    for (const auto &object: staticObjectsData) {
        if(object->name == "House"){
            newGameObject = Prefab::Instantiate<House>();
            newGameObject->transform->SetLocalPosition(object->position);
            newGameObject->transform->SetLocalRotation(object->rotation);
            newGameObject->transform->SetLocalScale(object->scale);
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

void SceneManager::ClearAllStaticObjects() {
    std::map<int, std::shared_ptr<SaveableStaticObject>> StaticObjects = FindAllStaticSaveablePrefabs();
;
    for(const auto& staticObject : StaticObjects){
        SceneManager::GetInstance()->activeScene->children.erase(staticObject.first);
    }
}
