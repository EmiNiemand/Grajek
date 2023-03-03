//
// Created by szymo on 02/03/2023.
//

#include "include/Factiories/GameObjectFactory.h"
#include "include/GloomEngine.h"
#include "include/HighLevelClasses/GameObject.h"

GameObjectFactory::GameObjectFactory(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

std::shared_ptr<GameObject> GameObjectFactory::CreateGameObject(std::string name, std::shared_ptr<GameObject> parent, Tags tag) {
    if(tag == Tags::SCENE) {
        parent = nullptr;
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(gloomEngine, name, parent, tag);
        gameObject->transform = std::make_shared<Transform>(parent);
        return gameObject;
    }
    if(parent == nullptr) parent = gloomEngine->activeScene;
    if(gloomEngine->FindGameObjectWithName(name) != nullptr) {
        bool endLoop = false;
        int i = 1;
        while(!endLoop) {
            name = name + std::to_string(i);
            if(gloomEngine->FindGameObjectWithName(name) == nullptr) endLoop = true;
            i++;
        }
    }
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(gloomEngine, name, parent, tag);
    parent->AddChild(gameObject);
    gameObject->transform = std::make_shared<Transform>(gameObject);
    gloomEngine->AddGameObject(gameObject);
    return gameObject;
}
