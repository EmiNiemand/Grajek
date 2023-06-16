//
// Created by Adrian on 05.04.2023.
//

#ifndef GLOOMENGINE_AIMANAGER_H
#define GLOOMENGINE_AIMANAGER_H

#include <memory>
#include <unordered_map>
#include "Components/Scripts/MusicPattern.h"
#include "Components/AI/CharacterPathfinding.h"

// Global AI consts
constexpr int AI_GRID_SIZE = 600;
constexpr float AI_AWARE_DISTANCE = 30.0f;
constexpr int AI_SPAWN_PLAYER_DISTANCE = 25;
constexpr int AI_SPAWN_GRID_DISTANCE = 15;
constexpr float AI_DESPAWN_TIMEOUT = 2.0f;
constexpr float AI_CELL_SIZE = 1.0f;
// Jazz Hood consts
constexpr float JAZZ_HOOD_DISTANCE = 70.0f;
constexpr int JAZZ_MAN_INCREASED_SPAWN_RATE = 5;
constexpr int JAZZ_MAN_DEFAULT_SPAWN_RATE = 2;
// Earnings modifiers
constexpr float CLAP_MODIFIER = 1.0f;
constexpr float DRUMS_MODIFIER = 1.5f;
constexpr float TRUMPET_MODIFIER = 2.0f;
constexpr float LAUNCHPAD_MODIFIER = 2.5f;
constexpr float GUITAR_MODIFIER = 3.0f;

class GloomEngine;
class CharacterLogic;
class CharacterMovement;
class CharacterPathfinding;
class BoxCollider;

class AIManager {
    int random = 0;
    // Hoods center points
    glm::vec3 jazzHoodCenter {};
    // Hoods spawn rates
    int jazzManSpawnRate = 0;
    // Player and enemy status
    std::shared_ptr<Transform> playerTransform = nullptr;
    glm::vec3 playerPosition {};
    InstrumentName currentPlayerInstrument {};
    bool playerIsPlaying = false;
    bool enemyIsPlaying = false;
    // Settings
    int charactersAmount = 0;

    inline static AIManager* aiManager;
    explicit AIManager();

public:
    std::unordered_map<int, std::shared_ptr<CharacterLogic>> charactersLogics;
    std::unordered_map<int, std::shared_ptr<CharacterMovement>> charactersMovements;
    std::shared_ptr<CharacterPathfinding> pathfinding;
    bool aiGrid[AI_GRID_SIZE * AI_GRID_SIZE] = {};
    bool isInitializing = true;
    // Spawn coords
    glm::ivec2 maxSpawnCoords {};
    glm::ivec2 minSpawnCoords {};

    AIManager(AIManager &other) = delete;
    void operator=(const AIManager&) = delete;
    virtual ~AIManager();

    static AIManager* GetInstance();
    void Free();

    void InitializeSpawner(const int& maxCharacters);
    void NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen);
    void NotifyPlayerStopsPlaying();
    void NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern> &pat);
    const float GetCombinedPlayerSatisfaction();
    void NotifyEnemyStartsPlaying(const InstrumentName &ins, const MusicGenre &gen);
    void NotifyEnemyStopsPlaying();
    void NotifyEnemyPlayedPattern(const std::shared_ptr<MusicPattern> &pat);
    const float GetCombinedEnemySatisfaction();
    [[nodiscard]] const int GetCharactersAmount() const;
    void SpawnCharacter();
    void RemoveCharacterLogic(const int& componentId);
    void RemoveCharacterMovement(const int &componentId);

};

#endif //GLOOMENGINE_AIMANAGER_H