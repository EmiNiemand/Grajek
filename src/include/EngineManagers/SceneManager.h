#ifndef OPENGLGP_SCENEMANAGER_H
#define OPENGLGP_SCENEMANAGER_H

#include <memory>
#include "nlohmann/json.hpp"
#include "LowLevelClasses/StaticObjectData.h"
#include "Interfaces/SaveableStaticObject.h"
#include <string>


class GloomEngine;
class GameObject;
//Insert prefab classes here
class House;
class Image;

class SceneManager {
private:
    inline static SceneManager* sceneManager;

public:
    std::shared_ptr<GameObject> activeScene;

    std::string file;

    std::shared_ptr<Image> loadingScreen;
    bool deleteLoadingScreen = false;

public:
    SceneManager(SceneManager &other) = delete;
    void operator=(const SceneManager&) = delete;
    virtual ~SceneManager();

    static SceneManager* GetInstance();

    void InitializeScene();
    void LoadScene(const std::string& scene);
    void ClearScene() const;
    void Free();


    void SaveStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName);
    void LoadStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName);
    void CreatePrefabObject(const std::string name);
    void CreatePrefabObject(const std::string name,const std::string modelPath);

#include "nlohmann/json.hpp"
private:
    explicit SceneManager();

    // Conversion of List of staticObjectData objects into json to be saved.
    void to_json(nlohmann::json &json, std::vector<std::shared_ptr<StaticObjectData>>& mapData);
    // Conversion of List of staticObjectData objects into json to be loaded.
    void from_json(const nlohmann::json &json, std::vector<std::shared_ptr<StaticObjectData>>& mapData);

    //IO function that reads map file and returns ready vector of pointers to staticObjData objects.
    std::vector<std::shared_ptr<StaticObjectData>> LoadMap(std::string dataDirectoryPath, std::string dataFileName);
    //IO function that saves map file from list of staticObjData
    void SaveMap(std::vector<std::shared_ptr<StaticObjectData>> mapData, std::string dataDirectoryPath,
                 std::string dataFileName);
    std::map<int, std::shared_ptr<SaveableStaticObject>> FindAllStaticSaveablePrefabs();
};


#endif //OPENGLGP_SCENEMANAGER_H
