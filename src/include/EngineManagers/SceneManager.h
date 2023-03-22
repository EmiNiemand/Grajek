#ifndef OPENGLGP_SCENEMANAGER_H
#define OPENGLGP_SCENEMANAGER_H

#include <memory>


class GloomEngine;
class GameObject;

class SceneManager {
public:
    std::shared_ptr<GloomEngine> gloomEngine;
    std::shared_ptr<GameObject> activeScene;

public:
    explicit SceneManager(const std::shared_ptr<GloomEngine> &gloomEngine);
    virtual ~SceneManager();

    void InitializeScene();
    //TODO: decide how to load scene and implement
    //void LoadScene();
    void ClearScene();
    void Free();
};


#endif //OPENGLGP_SCENEMANAGER_H
