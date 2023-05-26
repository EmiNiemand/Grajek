#include "Factories/GameObjectFactory.h"
#include "GloomEngine.h"
#include "EngineManagers/SceneManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Utilities.h"

GameObjectFactory::GameObjectFactory() = default;

GameObjectFactory::~GameObjectFactory() {
    delete gameObjectFactory;
}

GameObjectFactory* GameObjectFactory::GetInstance() {
    if (gameObjectFactory == nullptr) {
        gameObjectFactory = new GameObjectFactory();
    }
    return gameObjectFactory;
}

std::shared_ptr<GameObject> GameObjectFactory::CreateGameObject(std::string name, std::shared_ptr<GameObject> parent, Tags tag) {
    if(tag == Tags::SCENE) {
        int id = Utilities::Hash(name);

        if (GloomEngine::GetInstance()->FindGameObjectWithId(id) != nullptr) {
            while(true) {
                id++;
                if (GloomEngine::GetInstance()->FindGameObjectWithId(id) == nullptr) break;
            }
        }

        parent = nullptr;
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(name, id, parent, tag);
        gameObject->transform = std::make_shared<Transform>(gameObject);
        id++;
        gameObject->SetParent(gameObject);
        return gameObject;
    }
    if(parent == nullptr) parent = SceneManager::GetInstance()->activeScene;
    if(GloomEngine::GetInstance()->FindGameObjectWithName(name) != nullptr) {
        bool endLoop = false;
        int i = 1;
        while(!endLoop) {
            if (i == 1) name = name + std::to_string(i);
            else if (i > 1) name = name.substr(0, name.size() - std::to_string(i - 1).size()) + std::to_string(i);
            if(GloomEngine::GetInstance()->FindGameObjectWithName(name) == nullptr) endLoop = true;
            i++;
        }
    }
    int id = Utilities::Hash(name);

    if (GloomEngine::GetInstance()->FindGameObjectWithId(id) != nullptr) {
        while(true) {
            id++;
            if (GloomEngine::GetInstance()->FindGameObjectWithId(id) == nullptr) break;
        }
    }

    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(name, id, parent, tag);
    parent->AddChild(gameObject);
    gameObject->transform = std::make_shared<Transform>(gameObject);
    GloomEngine::GetInstance()->AddGameObject(gameObject);
    return gameObject;
}
