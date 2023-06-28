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
constexpr int AI_SPAWN_PLAYER_DISTANCE = 30;
constexpr int AI_SPAWN_X_MIN_DISTANCE = 27;
constexpr int AI_SPAWN_X_MAX_DISTANCE = 27;
constexpr int AI_SPAWN_Y_MIN_DISTANCE = 30;
constexpr int AI_SPAWN_Y_MAX_DISTANCE = 15;
constexpr int AI_POINT_DISTANCE = 25;
constexpr float AI_AWARE_DISTANCE = 35.0f;
constexpr float AI_DESPAWN_TIMEOUT = 1.1f;
constexpr float AI_CELL_SIZE = 1.0f;
// Jazz Hood consts
constexpr float JAZZ_HOOD_DISTANCE = 70.0f;
constexpr int JAZZ_MAN_INCREASED_SPAWN_RATE = 5;
constexpr int JAZZ_MAN_DEFAULT_SPAWN_RATE = 2;
// Earnings modifiers
constexpr float CLAP_MODIFIER = 1.0f;
constexpr float DRUMS_MODIFIER = 3.0f;
constexpr float TRUMPET_MODIFIER = 5.0f;
constexpr float LAUNCHPAD_MODIFIER = 8.0f;
constexpr float GUITAR_MODIFIER = 12.0f;

class GloomEngine;
class CharacterLogic;
class CharacterMovement;
class CharacterPathfinding;
class BoxCollider;

class AIManager {
    int random = 0;
    // Hoods spawn rates and center points
    std::pair<int, glm::vec3> jazzHoodParams;
    // Player and opponent status
    std::shared_ptr<Transform> playerTransform = nullptr;
    glm::vec3 playerPosition {};
    InstrumentName currentPlayerInstrument {};
    std::vector<std::shared_ptr<MusicPattern>> playerPatternsPlayed = {};
    InstrumentName currentOpponentInstrument {};
    bool isPlayerTalkingToOpponent = false;
    // Settings
    int charactersAmount = 0;
    std::vector<glm::vec3> positions;

    inline static AIManager* aiManager;
    explicit AIManager();

public:
    std::unordered_map<int, std::shared_ptr<CharacterLogic>> charactersLogics;
    std::unordered_map<int, std::shared_ptr<CharacterMovement>> charactersMovements;
    std::unordered_map<int, std::shared_ptr<CharacterMovement>> tempCharacters;
    std::unordered_map<int, glm::vec3> sessionPositions;
    std::shared_ptr<CharacterPathfinding> pathfinding;
    bool aiGrid[AI_GRID_SIZE * AI_GRID_SIZE] = {};
    bool isInitializing = true;
    float sessionCharacters = 0.0f;
    // Spawn coords
    glm::vec3 maxSpawnCoords {};
    glm::vec3 minSpawnCoords {};

    AIManager(AIManager &other) = delete;
    void operator=(const AIManager&) = delete;
    virtual ~AIManager();

    static AIManager* GetInstance();
    void Free();

    void InitializeSpawner(const int& maxCharacters);
    void SpawnCharacter();
    void NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen);
    void NotifyPlayerStopsPlaying();
    void NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern> &pat);
    const float GetCombinedPlayerSatisfaction();
    const float GetPlayerSkillLevel(const float &accuracy, const int &patternSize);
    void NotifyPlayerTalksWithOpponent(const bool& state);
    void NotifyOpponentStartsPlaying(const InstrumentName &ins, const MusicGenre &gen);
    void NotifyOpponentStopsPlaying();
    void NotifyOpponentPlayedPattern(const std::shared_ptr<MusicPattern> &pat);
    const float GetCombinedOpponentSatisfaction();
    const float GetOpponentSkillLevel(const float &accuracy, const int &patternSize);
    [[nodiscard]] const int GetCharactersAmount() const;
    void RemoveCharacterLogic(const int& componentId);

    void SelectPositions();
};

#endif //GLOOMENGINE_AIMANAGER_H