#ifndef OPENGLGP_SCENEMANAGER_H
#define OPENGLGP_SCENEMANAGER_H

#include <memory>
#include <vector>
#include <string>

class GloomEngine;
class GameObject;
class IStaticSaveable;

class SceneManager {
private:
    inline static SceneManager* sceneManager;

public:
    std::shared_ptr<GameObject> activeScene;

public:
    SceneManager(SceneManager &other) = delete;
    void operator=(const SceneManager&) = delete;
    virtual ~SceneManager();

    static SceneManager* GetInstance();

    void InitializeScene();
    //TODO: decide how to load scene and implement
    //void LoadScene();
    void ClearScene();
    void Free();

    void SaveStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName);
    void LoadStaticObjects(const std::string &dataDirectoryPath, const std::string &dataFileName);

private:
    std::vector<std::shared_ptr<IStaticSaveable>> FindAllStaticSaveableObjects();
    explicit SceneManager();
};


#endif //OPENGLGP_SCENEMANAGER_H
