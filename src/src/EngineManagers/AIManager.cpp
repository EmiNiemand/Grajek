//
// Created by Adrian on 05.04.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/AI/CharacterLogic.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/Default.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/RockDrums.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/JazzClap.h"

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
void AIManager::InitializeSpawner(const int& min, const int& max, const int& delay) {
#ifdef DEBUG
    ZoneScopedNC("AIManager", 0xDC143C);
#endif
    maxCharacters = max;
    spawnDelay = delay;
    pathfinding = std::make_shared<CharacterPathfinding>();

    int random;

    for (int i = 0; i < min; i++) {
        random = RandomnessManager::GetInstance()->GetInt(0, 2);

        switch (random) {
            case 0:
                Prefab::Instantiate<RockDrums>();
                break;
            case 1:
                Prefab::Instantiate<JazzClap>();
                break;
            default:
                Prefab::Instantiate<Default>();
                break;
        }
    }

    characterSpawner = std::jthread(SpawnCharacters, playerIsPlaying, min, maxCharacters, spawnDelay);
}

void AIManager::Free() {
    if (characterSpawner.joinable()) {
        characterSpawner.request_stop();
        characterSpawner.join();
    }
    charactersLogics.clear();
}

/**
 * @annotation
 * Notifies every character about started player session.
 * @param ins - enum of instrument name
 * @param gen - enum of music genre
 */
void AIManager::NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    mutex.lock();

    playerIsPlaying = true;

    for (auto&& ch : charactersLogics) {
        ch.second->SetPlayerInstrumentAndGenre(ins, gen);
        ch.second->SetPlayerPlayingStatus(true);
    }

    mutex.unlock();
}

/**
 * @annotation
 * Notifies every character about stopped player session.
 */
void AIManager::NotifyPlayerStopsPlaying() {
    mutex.lock();

    playerIsPlaying = false;

    for (auto&& ch : charactersLogics)
        ch.second->SetPlayerPlayingStatus(false);

    mutex.unlock();
}

/**
 * @annotation
 * Notifies every character about the pattern played by player.
 * @param pat - pointer to MusicPattern played by player
 */
void AIManager::NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    mutex.lock();

    for (auto&& ch : charactersLogics)
        ch.second->SetPlayerPattern(pat);

    mutex.unlock();
}

/**
 * @annotation
 * Returns combined player satisfaction.
 * @returns float - combined satisfaction of every character
 */
const float AIManager::GetCombinedPlayerSatisfaction() {
    float satisfaction = 0.0f;

    mutex.lock();

    for (auto&& ch : charactersLogics)
        satisfaction += ch.second->GetPlayerSatisfaction();

    if (!charactersLogics.empty())
        satisfaction /= (float)charactersLogics.size();

    mutex.unlock();

    return satisfaction;
}

/**
 * @annotation
 * Notifies every character about started enemy session.
 * @param ins - enum of instrument name
 * @param gen - enum of music genre
 */
void AIManager::NotifyEnemyStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    mutex.lock();

    enemyIsPlaying = true;

    for (auto&& ch : charactersLogics) {
        ch.second->SetEnemyInstrumentAndGenre(ins, gen);
        ch.second->SetEnemyPlayingStatus(true);
    }

    mutex.unlock();
}

/**
 * @annotation
 * Notifies every character about stopped player session.
 */
void AIManager::NotifyEnemyStopsPlaying() {
    mutex.lock();

    enemyIsPlaying = false;

    for (auto&& ch : charactersLogics)
        ch.second->SetEnemyPlayingStatus(false);

    mutex.unlock();
}

/**
 * @annotation
 * Notifies every character about the pattern played by enemy.
 * @param pat - pointer to MusicPattern played by enemy
 */
void AIManager::NotifyEnemyPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    mutex.lock();

    for (auto&& ch : charactersLogics)
        ch.second->SetEnemyPattern(pat);

    mutex.unlock();
}

/**
 * @annotation
 * Returns combined enemy satisfaction.
 * @returns float - combined satisfaction of every character
 */
const float AIManager::GetCombinedEnemySatisfaction() {
    float satisfaction = 0.0f;

    mutex.lock();

    for (auto&& ch : charactersLogics)
        satisfaction += ch.second->GetEnemySatisfaction();

    if (!charactersLogics.empty())
        satisfaction /= (float)charactersLogics.size();

    mutex.unlock();

    return satisfaction;
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
 * Removes CharacterMovement from the manager.
 * @param componentId - component id
 */
void AIManager::RemoveCharacterMovement(const int& componentId) {
    if (charactersMovements.contains(componentId))
        charactersMovements.erase(componentId);
}

/**
 * @annotation
 * Spawns character with the given time delay.
 * @param token - stop token
 * @param playerIsPlaying - references player status from manager
 * @param currentCharacters - already spawned amount of characters
 * @param maxCharacters - max amount of characters
 * @param spawnDelay - time delay between spawns
 */
void AIManager::SpawnCharacters(const std::stop_token& token, const bool& playerIsPlaying, const int& currentCharacters,
                                const int& maxCharacters, const int& spawnDelay) {

    auto delay = std::chrono::milliseconds(spawnDelay);
    int random, charactersAmount = currentCharacters;
    std::shared_ptr<GameObject> ch;

    while(!token.stop_requested()) {
        if (charactersAmount < maxCharacters) {
            random = RandomnessManager::GetInstance()->GetInt(0, 2);

            switch (random) {
                case 0:
                    ch = Prefab::Instantiate<RockDrums>();
                    break;
                case 1:
                    ch = Prefab::Instantiate<JazzClap>();
                    break;
                default:
                    ch = Prefab::Instantiate<Default>();
                    break;

            }

            if (playerIsPlaying)
                ch->GetComponent<CharacterLogic>()->SetPlayerPlayingStatus(true);

            ++charactersAmount;
        }

        std::this_thread::sleep_for(delay);
    }
}
