#include "include/Factories/GameObjectFactory.h"
#include "include/GloomEngine.h"
#include "include/GameObjectsAndPrefabs/GameObject.h"

GameObjectFactory::GameObjectFactory(const std::shared_ptr<GloomEngine> &gloomEngine) : gloomEngine(gloomEngine) {}

std::shared_ptr<GameObject> GameObjectFactory::CreateGameObject(std::string name, std::shared_ptr<GameObject> parent, Tags tag) {
    if(tag == Tags::SCENE) {
        parent = nullptr;
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(name, id, parent, tag);
        gameObject->transform = std::make_shared<Transform>(parent);
        id++;
        return gameObject;
    }
    if(parent == nullptr) parent = gloomEngine->activeScene;
    if(gloomEngine->FindGameObjectWithName(name) != nullptr) {
        bool endLoop = false;
        int i = 1;
        while(!endLoop) {
            if (i == 1) name = name + std::to_string(i);
            else if (i > 1) name = name.substr(0, name.size() - 1) + std::to_string(i);
            if(gloomEngine->FindGameObjectWithName(name) == nullptr) endLoop = true;
            i++;
        }
    }
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(name, id, parent, tag);
    parent->AddChild(gameObject);
    gameObject->transform = std::make_shared<Transform>(gameObject);
    gloomEngine->AddGameObject(gameObject);
    id++;
    return gameObject;
}
