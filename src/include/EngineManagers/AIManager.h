//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AIMANAGER_H
#define GLOOMENGINE_AIMANAGER_H

#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "Components/Scripts/MusicPattern.h"
#include "Components/AI/CharacterPathfinding.h"

constexpr int AI_GRID_SIZE = 100;

class GloomEngine;
class CharacterLogic;
class CharacterPathfinding;
class BoxCollider;

class AIManager {
    bool playerIsPlaying = false;
    int maxCharacters;
    int spawnDelay;
    std::jthread characterSpawner;
    std::mutex mutex;

    inline static AIManager* aiManager;
    explicit AIManager();

    static void SpawnCharacters(const std::stop_token& token, std::mutex& mutex, const bool& playerIsPlaying,
                                const int& maxCharacters, const int& spawnDelay,
                                std::unordered_map<int, std::shared_ptr<CharacterLogic>>* currentCharactersLogics);

public:
    //TODO: Implement!
    //std::vector<std::shared_ptr<GameObject>> buildings;
    std::unordered_map<int, std::shared_ptr<CharacterLogic>> currentCharactersLogics;
    std::shared_ptr<CharacterPathfinding> pathfinding;
    const float aiGridSize = 1.0f;
    bool aiGrid[AI_GRID_SIZE][AI_GRID_SIZE] = {};

    AIManager(AIManager &other) = delete;
    void operator=(const AIManager&) = delete;
    virtual ~AIManager();

    static AIManager* GetInstance();
    void Free();

    void InitializeSpawner(const int& min, const int& max, const int& delay);
    void NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen);
    void NotifyPlayerStopsPlaying();
    void NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern> &pat);
    const float GetCombinedSatisfaction();
    void RemoveBoxCollider(const std::shared_ptr<BoxCollider>& ptr) const;

};

#endif //GLOOMENGINE_AIMANAGER_H