//
// Created by Adrian on 05.04.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/Default.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/JazzTrumpet.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

AIManager::AIManager() {
    playerPatternsPlayed.reserve(3);
}

AIManager::~AIManager() {
    delete aiManager;
}

AIManager* AIManager::GetInstance() {
    return (aiManager == nullptr) ? aiManager = new AIManager() : aiManager;
}

/**
 * @annotation
 * Initializes manager and spawner.
 * @param maxCharacters - max amount of characters
 */
void AIManager::InitializeSpawner(const int& maxCharacters) {
#ifdef DEBUG
    ZoneScopedNC("AIManager", 0xDC143C);
#endif

    isInitializing = true;
    charactersAmount = maxCharacters;
    charactersLogics.reserve(maxCharacters);
    charactersMovements.reserve(maxCharacters);
    tempCharacters.reserve(maxCharacters);
    sessionPositions.reserve(maxCharacters);
    positions.reserve(maxCharacters);
    pathfinding = std::make_shared<CharacterPathfinding>();
    jazzHoodParams.second = GloomEngine::GetInstance()->FindGameObjectWithName("JazzHoodCenter")->
            transform->GetLocalPosition();

    minSpawnCoords = GloomEngine::GetInstance()->FindGameObjectWithName("MinSpawnCoords")->transform->GetLocalPosition();
    minSpawnCoords.x = std::clamp(minSpawnCoords.x, -AI_GRID_SIZE / 2.0f, AI_GRID_SIZE / 2.0f);
    minSpawnCoords.z = std::clamp(minSpawnCoords.z, -AI_GRID_SIZE / 2.0f, AI_GRID_SIZE / 2.0f);

    maxSpawnCoords = GloomEngine::GetInstance()->FindGameObjectWithName("MaxSpawnCoords")->transform->GetLocalPosition();
    maxSpawnCoords.x = std::clamp(maxSpawnCoords.x, -AI_GRID_SIZE / 2.0f, AI_GRID_SIZE / 2.0f);
    maxSpawnCoords.z = std::clamp(maxSpawnCoords.z, -AI_GRID_SIZE / 2.0f, AI_GRID_SIZE / 2.0f);

    playerTransform = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
    playerPosition = playerTransform->GetLocalPosition();

    for (int i = 0; i < charactersAmount; i++) {
        random = RandomnessManager::GetInstance()->GetInt(1, 10);

        if (glm::distance(playerPosition, jazzHoodParams.second) < JAZZ_HOOD_DISTANCE)
            jazzHoodParams.first = JAZZ_MAN_INCREASED_SPAWN_RATE;
        else
            jazzHoodParams.first = JAZZ_MAN_DEFAULT_SPAWN_RATE;

        if (random <= jazzHoodParams.first)
            Prefab::Instantiate<JazzTrumpet>();
        else
            Prefab::Instantiate<Default>();
    }
}

void AIManager::Free() {
    charactersLogics.clear();
    charactersMovements.clear();
    tempCharacters.clear();
    sessionPositions.clear();
    positions.clear();
    playerPatternsPlayed.clear();
    playerTransform = nullptr;
}

/**
 * @annotation
 * Selects positions near player for every character.
 */
void AIManager::SelectPositions() {
    sessionPositions.clear();
    playerPosition = playerTransform->GetLocalPosition();
    glm::ivec2 newEndPoint = {std::round(playerPosition.x), std::round(playerPosition.z)};
    int boundaries = 2;
    bool isAvailable = true;

    int i = 0;
    for (int y = -boundaries; y <= boundaries; ++y) {
        for (int x = -boundaries; x <= boundaries; ++x) {
            if (y == -boundaries || y == boundaries || x == -boundaries || x == boundaries) {
                if (!aiGrid[(newEndPoint.x + x + AI_GRID_SIZE / 2) + (newEndPoint.y + y + AI_GRID_SIZE / 2) * AI_GRID_SIZE]) {
                    positions.emplace_back(newEndPoint.x + x, 0.0f, newEndPoint.y + y);

                    if (++i == charactersAmount)
                        isAvailable = false;
                }

                if (!isAvailable)
                    break;
            }
        }

        if (!isAvailable)
            break;

        if (y >= boundaries) {
            ++boundaries;
            y = -boundaries;
        }
    }

    float maxDistance, distance;
    int iterator;
    for (const auto& mov : charactersMovements) {
        maxDistance = FLT_MAX;
        iterator = 0;

        for (i = 0; i < positions.size(); ++i) {
            distance = glm::distance(positions[i], mov.second->GetCurrentPosition());

            if (distance < maxDistance) {
                maxDistance = distance;
                iterator = i;
            }
        }

        sessionPositions[mov.first] = positions[iterator];
        positions.erase(positions.begin() + iterator);
    }
}

/**
 * @annotation
 * Notifies every character about started player session.
 * @param ins - enum of instrument name
 * @param gen - enum of music genre
 */
void AIManager::NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    sessionCharacters = 0.0f;

    currentPlayerInstrument = ins;

    for (auto&& ch : charactersLogics) {
        ch.second->SetPlayerInstrumentAndGenre(ins, gen);
        ch.second->SetPlayerPlayingStatus(true);
    }

    if (!isPlayerTalkingToOpponent)
        SelectPositions();
}

/**
 * @annotation
 * Notifies every character about stopped player session.
 */
void AIManager::NotifyPlayerStopsPlaying() {
    currentPlayerInstrument = {};

    for (auto&& ch : charactersLogics)
        ch.second->SetPlayerPlayingStatus(false);
}

/**
 * @annotation
 * Notifies every character about the pattern played by player.
 * @param pat - pointer to MusicPattern played by player
 */
void AIManager::NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    AI_LOGIC_STATE state;

    if (playerPatternsPlayed.size() == 3)
        playerPatternsPlayed.pop_back();

    playerPatternsPlayed.insert(playerPatternsPlayed.begin(), pat);

    for (auto &&ch: charactersLogics) {
        state = ch.second->GetLogicState();

        if (state == ListeningToPlayer || state == ListeningToDuel || state == WalkingAway)
            ch.second->SetPlayerPattern(pat);
    }
}

/**
 * @annotation
 * Returns combined player satisfaction.
 * @returns float - combined satisfaction of every character
 */
const float AIManager::GetCombinedPlayerSatisfaction() {
    float satisfaction = 0.0f;
    AI_LOGIC_STATE state;

    for (auto&& ch : charactersLogics) {
        state = ch.second->GetLogicState();

        if (state == ListeningToPlayer || state == ListeningToDuel) {
            satisfaction += ch.second->GetPlayerSatisfaction();
            sessionCharacters += 1.0f;
        }
    }

    if (sessionCharacters != 0.0f) {
        satisfaction /= sessionCharacters;

        switch (currentPlayerInstrument) {
            case Clap:
                satisfaction *= CLAP_MODIFIER;
                break;
            case Drums:
                satisfaction *= DRUMS_MODIFIER;
                break;
            case Trumpet:
                satisfaction *= TRUMPET_MODIFIER;
                break;
            case Launchpad:
                satisfaction *= LAUNCHPAD_MODIFIER;
                break;
            case Guitar:
                satisfaction *= GUITAR_MODIFIER;
                break;
        }
    }

    return satisfaction;
}

/**
 * @annotation
 * Returns player skill level based on equation:
 * ((accuracyValue * (float)patternSize) + satisfaction / 50.0f * randomModifier) * patternModifier
 * @param accuracy - latest player pattern accuracy
 * @param patternSize - pattern array size
 * @returns float - total player skill level.
 */
const float AIManager::GetPlayerSkillLevel(const float& accuracy, const int& patternSize) {
    float randomModifier = RandomnessManager::GetInstance()->GetFloat(0.90f, 1.10f);
    float satisfaction = GetCombinedPlayerSatisfaction();
    float accuracyValue = accuracy;
    float patternModifier = 1.0f;

    satisfaction *= std::floor(sessionCharacters / 5.0f) * 0.5f;

    if (accuracy <= 0.5)
        accuracyValue = 0;
    else if (accuracy <= 0.8)
        accuracyValue = 0.25;
    else if (accuracy <= 0.95)
        accuracyValue = 1.0;
    else if (accuracy <= 1)
        accuracyValue = 1.5;

    auto playerPlayedPattern = playerPatternsPlayed[0];

    for (int i = 1; i < 3; ++i) {
        if (playerPatternsPlayed.size() > i && playerPatternsPlayed[i] == playerPlayedPattern)
            patternModifier -= 0.3f;
    }

    return ((accuracyValue * (float)patternSize) + satisfaction / 50.0f * randomModifier) * patternModifier;
}

/**
 * @annotation
 * Notifies every character that player is talking to the opponent.
 * @param state - true is player is talking
 */
void AIManager::NotifyPlayerTalksWithOpponent(const bool& state) {
    isPlayerTalkingToOpponent = state;

    for (auto&& ch : charactersLogics)
        ch.second->SetOpponentPlayingStatus(state);

    if (isPlayerTalkingToOpponent)
        SelectPositions();
}

/**
 * @annotation
 * Notifies every character about started opponent session.
 * @param ins - enum of instrument name
 * @param gen - enum of music genre
 */
void AIManager::NotifyOpponentStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    currentOpponentInstrument = ins;

    for (auto&& ch : charactersLogics)
        ch.second->SetOpponentInstrumentAndGenre(ins, gen);
}

/**
 * @annotation
 * Notifies every character about stopped opponent session.
 */
void AIManager::NotifyOpponentStopsPlaying() {
    currentOpponentInstrument = {};

    for (auto&& ch : charactersLogics)
        ch.second->SetOpponentPlayingStatus(false);
}

/**
 * @annotation
 * Notifies every character about the pattern played by opponent.
 * @param pat - pointer to MusicPattern played by opponent
 */
void AIManager::NotifyOpponentPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    AI_LOGIC_STATE state;

    for (auto&& ch : charactersLogics) {
        state = ch.second->GetLogicState();

        if (state == ListeningToDuel || state == WalkingAway)
            ch.second->SetOpponentPattern(pat);
    }
}

/**
 * @annotation
 * Returns combined opponent satisfaction.
 * @returns float - combined satisfaction of every character
 */
const float AIManager::GetCombinedOpponentSatisfaction() {
    float satisfaction = 0.0f;
    AI_LOGIC_STATE state;

    for (auto &&ch: charactersLogics) {
        state = ch.second->GetLogicState();

        if (state == ListeningToPlayer || state == ListeningToDuel)
            satisfaction += ch.second->GetOpponentSatisfaction();
    }

    if (sessionCharacters != 0.0f) {
        satisfaction /= sessionCharacters;

        switch (currentOpponentInstrument) {
            case Clap:
                satisfaction *= CLAP_MODIFIER;
                break;
            case Drums:
                satisfaction *= DRUMS_MODIFIER;
                break;
            case Trumpet:
                satisfaction *= TRUMPET_MODIFIER;
                break;
            case Launchpad:
                satisfaction *= LAUNCHPAD_MODIFIER;
                break;
            case Guitar:
                satisfaction *= GUITAR_MODIFIER;
                break;
        }
    }

    return satisfaction;
}

/**
 * @annotation
 * Returns opponent skill level based on equation:
 * ((accuracy * (float)patternSize) + satisfaction / 50.0f * randomModifier)
 * @param accuracy - latest opponent pattern accuracy
 * @param patternSize - pattern array size
 * @returns float - total opponent skill level.
 */
const float AIManager::GetOpponentSkillLevel(const float& accuracy, const int& patternSize) {
    float randomModifier = RandomnessManager::GetInstance()->GetFloat(0.90f, 1.10f);
    float satisfaction = GetCombinedOpponentSatisfaction();

    satisfaction *= std::floor(sessionCharacters / 5.0f) * 0.5f;

    return ((accuracy * (float)patternSize) + satisfaction / 50.0f * randomModifier);
}

/**
 * @annotation
 * Returns max characters value
 * @returns int - maxCharacters
 */
const int AIManager::GetCharactersAmount() const {
    return charactersAmount;
}

/**
 * @annotation
 * Removes CharacterLogic from the manager.
 * @param componentId - component id
 */
void AIManager::RemoveCharacterLogic(const int& componentId) {
    if (charactersLogics.contains(componentId))
        charactersLogics.erase(componentId);
}

/**
 * @annotation
 * Spawns new character.
 */
void AIManager::SpawnCharacter() {
    isInitializing = false;

    random = RandomnessManager::GetInstance()->GetInt(1, 10);

    if (glm::distance(playerTransform->GetLocalPosition(), jazzHoodParams.second) < JAZZ_HOOD_DISTANCE)
        jazzHoodParams.first = JAZZ_MAN_INCREASED_SPAWN_RATE;
    else
        jazzHoodParams.first = JAZZ_MAN_DEFAULT_SPAWN_RATE;

    if (random <= jazzHoodParams.first)
        Prefab::Instantiate<JazzTrumpet>();
    else
        Prefab::Instantiate<Default>();
}
