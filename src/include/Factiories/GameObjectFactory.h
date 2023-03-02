//
// Created by szymo on 02/03/2023.
//

#ifndef OPENGLGP_GAMEOBJECTFACTORY_H
#define OPENGLGP_GAMEOBJECTFACTORY_H

#include "include/ProjectSettings.h"
#include <memory>
#include <string>

class GloomEngine;
class GameObject;

class GameObjectFactory {
private:
    std::shared_ptr<GloomEngine> gloomEngine;
public:
    explicit GameObjectFactory(const std::shared_ptr<GloomEngine> &gloomEngine);

    std::shared_ptr<GameObject> CreateGameObject(std::string name, std::shared_ptr<GameObject> parent = nullptr, Tags tag = Tags::DEFAULT);
};


#endif //OPENGLGP_GAMEOBJECTFACTORY_H
