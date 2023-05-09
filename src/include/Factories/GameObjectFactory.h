#ifndef OPENGLGP_GAMEOBJECTFACTORY_H
#define OPENGLGP_GAMEOBJECTFACTORY_H

#include "ProjectSettings.h"
#include <memory>
#include <string>

class GloomEngine;
class GameObject;

class GameObjectFactory {
private:
    inline static GameObjectFactory* gameObjectFactory = nullptr;

public:
    GameObjectFactory(GameObjectFactory &other) = delete;
    void operator=(const GameObjectFactory&) = delete;
    std::shared_ptr<GameObject> CreateGameObject(std::string name, std::shared_ptr<GameObject> parent = nullptr, Tags tag = Tags::DEFAULT);

    static GameObjectFactory* GetInstance();

private:
    explicit GameObjectFactory();
};


#endif //OPENGLGP_GAMEOBJECTFACTORY_H
