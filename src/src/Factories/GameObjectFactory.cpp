//
// Created by szymo on 02/03/2023.
//

#include "include/Factiories/GameObjectFactory.h"
#include "include/GloomEngine.h"
#include "include/HighLevelClasses/GameObject.h"

GameObjectFactory::GameObjectFactory(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

std::shared_ptr<GameObject> GameObjectFactory::CreateGameObject(std::string name, std::shared_ptr<GameObject> parent, Tags tag) {
    if(parent == nullptr) parent = gloomEngine->scene;
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(gloomEngine, name, parent, tag);
    gloomEngine->AddGameObject(gameObject);
    return gameObject;
}
