#ifndef OPENGLGP_SCENEMANAGER_H
#define OPENGLGP_SCENEMANAGER_H

#include <memory>
#include <string>


class GloomEngine;
class GameObject;

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
    void LoadScene(const std::string& scene);
    void ClearScene() const;
    void Free();

private:
    explicit SceneManager();
};


#endif //OPENGLGP_SCENEMANAGER_H
