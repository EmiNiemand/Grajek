//
// Created by szymo on 04/05/2023.
//

#ifndef GLOOMENGINE_PREFABFACTORY_H
#define GLOOMENGINE_PREFABFACTORY_H

#include <memory>
#include <string>

#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/SceneManager.h"
#include "ProjectSettings.h"
#include "Utilities.h"

class Player;
class Die;
class Shop;
class House;

class PrefabFactory {
private:
    inline static PrefabFactory* prefabFactory = nullptr;

public:
    PrefabFactory(PrefabFactory &other) = delete;
    void operator=(const PrefabFactory&) = delete;

    template<class T>
    std::shared_ptr<GameObject> CreateGameObjectFromPrefab(const std::string& name, std::shared_ptr<GameObject> parent = nullptr,
                                                  Tags tag = Tags::DEFAULT) {
        std::string newName = name.substr(0, 5);
        if (name.length() > 5 && newName == "class") {
            newName = name.substr(6, name.length());
        }
        else {
            newName = name;
        }

        if(parent == nullptr) parent = SceneManager::GetInstance()->activeScene;
        if(GloomEngine::GetInstance()->FindGameObjectWithName(newName) != nullptr) {
            bool endLoop = false;
            int i = 1;
            while(!endLoop) {
                if (i == 1) newName += std::to_string(i);
                else if (i > 1) newName = newName.substr(0, newName.size() - std::to_string(i - 1).size()) + std::to_string(i);
                if(GloomEngine::GetInstance()->FindGameObjectWithName(newName) == nullptr) endLoop = true;
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

        std::shared_ptr<T> prefab = std::make_shared<T>(newName, id, parent, tag);
        parent->AddChild(prefab);
        prefab->transform = std::make_shared<Transform>(prefab);
        GloomEngine::GetInstance()->AddGameObject(prefab);
        prefab->Create();
        return prefab;
    }

    static PrefabFactory* GetInstance();

private:
    explicit PrefabFactory();
};


#endif //GLOOMENGINE_PREFABFACTORY_H
