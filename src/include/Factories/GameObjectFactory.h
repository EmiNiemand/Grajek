#ifndef OPENGLGP_GAMEOBJECTFACTORY_H
#define OPENGLGP_GAMEOBJECTFACTORY_H

#include "ProjectSettings.h"
#include <memory>
#include <string>

class GloomEngine;
class GameObject;

class GameObjectFactory {
private:
    int id = 0;
    std::shared_ptr<GloomEngine> gloomEngine;
public:
    explicit GameObjectFactory(const std::shared_ptr<GloomEngine> &gloomEngine);

    std::shared_ptr<GameObject> CreateGameObject(std::string name, std::shared_ptr<GameObject> parent = nullptr, Tags tag = Tags::DEFAULT);
};


#endif //OPENGLGP_GAMEOBJECTFACTORY_H
