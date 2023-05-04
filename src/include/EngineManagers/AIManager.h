//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AIMANAGER_H
#define GLOOMENGINE_AIMANAGER_H

#include <memory>
#include <unordered_map>
#include <thread>
#include "GameObjectsAndPrefabs/Prefab.h"

class GloomEngine;
class GameObject;

class AIManager {
    float spawnDistance;
    int maxCharacters;
    int spawnDelay;
    std::jthread characterSpawner;

    inline static AIManager* aiManager;
    explicit AIManager();

    static void SpawnCharacters(const std::stop_token& token, const float& spawnDistance, const int& maxCharacters,
                                const int& spawnDelay, const std::vector<std::shared_ptr<GameObject> (*)(std::string)>& charactersPrefabs,
                                std::unordered_map<int, std::shared_ptr<GameObject>>* currentCharacters);

public:
    //TODO: Implement!
    //std::vector<std::shared_ptr<GameObject>> buildings;
    std::vector<std::shared_ptr<GameObject> (*)(std::string)> charactersPrefabs;
    std::unordered_map<uint32_t, std::shared_ptr<GameObject>> currentCharacters;

    AIManager(AIManager &other) = delete;
    void operator=(const AIManager&) = delete;
    virtual ~AIManager();

    static AIManager* GetInstance();
    void Free();

    void InitializeSpawner(const float& dist, const int& min, const int& max, const int& delay);

};

#endif //GLOOMENGINE_AIMANAGER_H
