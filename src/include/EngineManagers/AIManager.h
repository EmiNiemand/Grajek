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

// Global AI consts
constexpr int AI_GRID_SIZE = 600;
constexpr float AI_AWARE_DISTANCE = 30.0f;
constexpr int AI_SPAWN_GRID_DISTANCE = 25;
constexpr float AI_DESPAWN_TIMEOUT = 2.0f;
constexpr float AI_CELL_SIZE = 1.0f;
// Jazz Hood consts
constexpr float JAZZ_HOOD_DISTANCE = 75.0f;
constexpr int JAZZ_MAN_INCREASED_SPAWN_RATE = 5;
constexpr int JAZZ_MAN_DEFAULT_SPAWN_RATE = 2;

class GloomEngine;
class CharacterLogic;
class CharacterMovement;
class CharacterPathfinding;
class BoxCollider;

class AIManager {
    // Hoods
    glm::vec3 jazzHoodCenter {};
    // Player and enemy status
    bool playerIsPlaying = false;
    bool enemyIsPlaying = false;
    // Settings
    int currentCharacters = 0;
    int maxCharacters = 0;
    int spawnDelay = 0;
    std::jthread characterSpawner;
    std::mutex mutex;

    inline static AIManager* aiManager;
    explicit AIManager();

    static void SpawnCharacters(const std::stop_token& token, const bool& playerIsPlaying, int& currentCharacters,
                                const int& maxCharacters, const int& spawnDelay, const glm::vec3& jazzHoodCenter);

public:
    //TODO: Implement!
    //std::vector<std::shared_ptr<GameObject>> buildings;
    std::unordered_map<int, std::shared_ptr<CharacterLogic>> charactersLogics;
    std::unordered_map<int, std::shared_ptr<CharacterMovement>> charactersMovements;
    std::shared_ptr<CharacterPathfinding> pathfinding;
    bool aiGrid[AI_GRID_SIZE * AI_GRID_SIZE] = {};
    bool isInitializing = true;

    AIManager(AIManager &other) = delete;
    void operator=(const AIManager&) = delete;
    virtual ~AIManager();

    static AIManager* GetInstance();
    void Free();

    void InitializeSpawner(const int& min, const int& max, const int& delay);
    void NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen);
    void NotifyPlayerStopsPlaying();
    void NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern> &pat);
    const float GetCombinedPlayerSatisfaction();
    void NotifyEnemyStartsPlaying(const InstrumentName &ins, const MusicGenre &gen);
    void NotifyEnemyStopsPlaying();
    void NotifyEnemyPlayedPattern(const std::shared_ptr<MusicPattern> &pat);
    const float GetCombinedEnemySatisfaction();
    [[nodiscard]] const int GetMaxCharacters() const;
    void RemoveCharacter(const std::shared_ptr<GameObject>& gameObject);
    void RemoveCharacterLogic(const int& componentId);
    void RemoveCharacterMovement(const int &componentId);

};

#endif //GLOOMENGINE_AIMANAGER_H