//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AIMANAGER_H
#define GLOOMENGINE_AIMANAGER_H

#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "GameObjectsAndPrefabs/Prefab.h"

class GloomEngine;
class GameObject;

class AIManager {
    bool playerIsPlaying = false;
    int maxCharacters;
    int spawnDelay;
    std::jthread characterSpawner;
    std::mutex mutex;

    inline static AIManager* aiManager;
    explicit AIManager();

    static void SpawnCharacters(const std::stop_token& token, const bool& playerIsPlaying, const int& maxCharacters,
                                const int& spawnDelay, const std::vector<std::shared_ptr<GameObject> (*)(std::string)>& charactersPrefabs,
                                std::unordered_map<int, std::shared_ptr<GameObject>>* currentCharacters);

public:
    //TODO: Implement!
    //std::vector<std::shared_ptr<GameObject>> buildings;
    std::vector<std::shared_ptr<GameObject> (*)(std::string)> charactersPrefabs;
    std::unordered_map<int, std::shared_ptr<GameObject>> currentCharacters;

    AIManager(AIManager &other) = delete;
    void operator=(const AIManager&) = delete;
    virtual ~AIManager();

    static AIManager* GetInstance();
    void Free();

    void InitializeSpawner(const int& min, const int& max, const int& delay);
    void NotifyPlayerStartsPlaying();
    void NotifyPlayerStopsPlaying();
	void NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern> &pat);

};

#endif //GLOOMENGINE_AIMANAGER_H
