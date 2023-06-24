//
// Created by Adrian on 05.04.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/AI/CharacterLogic.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/Default.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/JazzTrumpet.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

AIManager::AIManager() = default;

AIManager::~AIManager() {
    delete aiManager;
}

AIManager* AIManager::GetInstance() {
    return (aiManager == nullptr) ? aiManager = new AIManager() : aiManager;
}

/**
 * @annotation
 * Initializes manager and spawner. Also creates new thread
 * which periodically spawns new entities.
 * @param min - min amount of entities
 * @param max - max amount of entities
 * @param delay - time delay between spawns
 */
void AIManager::InitializeSpawner(const int& maxCharacters) {
#ifdef DEBUG
    ZoneScopedNC("AIManager", 0xDC143C);
#endif

    isInitializing = true;
    charactersAmount = maxCharacters;
    charactersLogics.reserve(maxCharacters);
    charactersMovements.reserve(maxCharacters);
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
    playerTransform = nullptr;
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
}

/**
 * @annotation
 * Notifies every character about stopped player session.
 */
void AIManager::NotifyPlayerStopsPlaying() {
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

    for (auto &&ch: charactersLogics) {
        state = ch.second->GetLogicState();

        if (state == Listening || state == WalkingAway)
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

        if (state == Listening) {
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

float AIManager::GetReward(const float& accuracy, const int& patternSize) {
    float randomModifier = RandomnessManager::GetInstance()->GetFloat(0.90f, 1.10f);
    float satisfaction = GetCombinedPlayerSatisfaction();

    if (sessionCharacters <= 10.0f)
        satisfaction *= 1.25;
    else if (sessionCharacters <= 20.0f)
        satisfaction *= 1.5f;
    else if (sessionCharacters <= 30.0f)
        satisfaction *= 1.75f;
    else
        satisfaction *= 2.0f;

    return ((accuracy * (float)patternSize) + satisfaction / 100.0f * randomModifier);
}

void AIManager::NotifyPlayerTalksWithOpponent(const bool& state) {
    for (auto&& ch : charactersLogics)
        ch.second->SetOpponentPlayingStatus(state);
}

/**
 * @annotation
 * Notifies every character about started enemy session.
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
 * Notifies every character about stopped player session.
 */
void AIManager::NotifyOpponentStopsPlaying() {
    for (auto&& ch : charactersLogics)
        ch.second->SetOpponentPlayingStatus(false);
}

/**
 * @annotation
 * Notifies every character about the pattern played by enemy.
 * @param pat - pointer to MusicPattern played by enemy
 */
void AIManager::NotifyOpponentPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    AI_LOGIC_STATE state;

    for (auto&& ch : charactersLogics) {
        state = ch.second->GetLogicState();

        if (state == Listening)
            ch.second->SetOpponentPattern(pat);
    }
}

/**
 * @annotation
 * Returns combined enemy satisfaction.
 * @returns float - combined satisfaction of every character
 */
const float AIManager::GetCombinedOpponentSatisfaction(const float& accuracy, const int& patternSize) {
    float satisfaction = 0.0f;
    float characterCounter = 0.0f;
    AI_LOGIC_STATE state;

    for (auto&& ch : charactersLogics) {
        state = ch.second->GetLogicState();

        if (state == Listening) {
            satisfaction += ch.second->GetOpponentSatisfaction();
            characterCounter += 1.0f;
        }
    }

    if (characterCounter != 0.0f) {
        satisfaction /= characterCounter;

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

    float randomModifier = RandomnessManager::GetInstance()->GetFloat(0.90f, 1.10f);

    if (sessionCharacters <= 10.0f)
        satisfaction *= 1.25;
    else if (sessionCharacters <= 20.0f)
        satisfaction *= 1.5f;
    else if (sessionCharacters <= 30.0f)
        satisfaction *= 1.75f;
    else
        satisfaction *= 2.0f;

    return ((accuracy / 100 * (float)patternSize) + satisfaction / 100.0f * randomModifier);
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
