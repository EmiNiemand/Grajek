//
// Created by Adrian on 05.04.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
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
void AIManager::InitializeSpawner(const int& min, const int& max, const int& delay) {
#ifdef DEBUG
    ZoneScopedNC("AIManager", 0xDC143C);
#endif

    isInitializing = true;
    currentCharacters = min;
    maxCharacters = max;
    spawnDelay = delay;
    pathfinding = std::make_shared<CharacterPathfinding>();
    jazzHoodCenter = GloomEngine::GetInstance()->FindGameObjectWithName("JazzHoodCenter")->
            transform->GetLocalPosition();
    glm::vec3 playerPosition = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->
            transform->GetLocalPosition();

    int random, jazzManSpawnRate;

    for (int i = 0; i < min; i++) {
        random = RandomnessManager::GetInstance()->GetInt(1, 10);

        if (glm::distance(playerPosition, jazzHoodCenter) < JAZZ_HOOD_DISTANCE)
            jazzManSpawnRate = JAZZ_MAN_INCREASED_SPAWN_RATE;
        else
            jazzManSpawnRate = JAZZ_MAN_DEFAULT_SPAWN_RATE;

        if (random <= jazzManSpawnRate)
            Prefab::Instantiate<JazzTrumpet>();
        else
            Prefab::Instantiate<Default>();
    }

    isInitializing = false;

    characterSpawner = std::jthread(SpawnCharacters, playerIsPlaying, std::ref(currentCharacters), maxCharacters,
                                    spawnDelay, jazzHoodCenter);
}

void AIManager::Free() {
    if (characterSpawner.joinable()) {
        characterSpawner.request_stop();
        characterSpawner.join();
    }
    charactersLogics.clear();
    charactersMovements.clear();
}

/**
 * @annotation
 * Notifies every character about started player session.
 * @param ins - enum of instrument name
 * @param gen - enum of music genre
 */
void AIManager::NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    playerIsPlaying = true;

    mutex.lock();

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
    playerIsPlaying = false;

    mutex.lock();

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
    // TODO: improve varied satisfaction
//    float satisfactionModifier;
//
//    switch (currentPlayerInstrument) {
//        case Clap:
//            satisfactionModifier = RandomnessManager::GetInstance()->GetFloat(10, 45);
//            break;
//        case Drums:
//            satisfactionModifier = 63.0f;
//            break;
//        default:
//            satisfactionModifier = RandomnessManager::GetInstance()->GetFloat(0, 45);
//            break;
//    }

    mutex.lock();

    for (auto&& ch : charactersLogics)
        satisfaction += ch.second->GetPlayerSatisfaction();

    if (!charactersLogics.empty())
        satisfaction /= (float)charactersLogics.size(); // * satisfactionModifier / 100.0f;

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
    enemyIsPlaying = true;

    mutex.lock();

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
    enemyIsPlaying = false;

    mutex.lock();

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
 * Returns max characters value
 * @returns int - maxCharacters
 */
const int AIManager::GetMaxCharacters() const {
    return maxCharacters;
}

/**
 * @annotation
 * Removes Character completely from the manager.
 * @param componentId - component id
 */
void AIManager::RemoveCharacter(const std::shared_ptr<GameObject>& character) {
    GloomEngine::GetInstance()->AddGameObjectToDestroyBuffer(character);

    mutex.lock();

    --currentCharacters;

    mutex.unlock();
}

/**
 * @annotation
 * Removes CharacterLogic from the manager.
 * @param componentId - component id
 */
void AIManager::RemoveCharacterLogic(const int& componentId) {
    mutex.lock();

    if (charactersLogics.contains(componentId))
        charactersLogics.erase(componentId);

    mutex.unlock();
}

/**
 * @annotation
 * Removes CharacterMovement from the manager.
 * @param componentId - component id
 */
void AIManager::RemoveCharacterMovement(const int& componentId) {
    mutex.lock();

    if (charactersMovements.contains(componentId))
        charactersMovements.erase(componentId);

    mutex.unlock();
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
void AIManager::SpawnCharacters(const std::stop_token& token, const bool& playerIsPlaying, int& currentCharacters,
                                const int& maxCharacters, const int& spawnDelay, const glm::vec3& jazzHoodCenter) {

    auto delay = std::chrono::milliseconds(spawnDelay);
    int random, jazzManSpawnRate;
    std::shared_ptr<GameObject> ch;
    std::shared_ptr<Transform> playerTransform = GloomEngine::GetInstance()->
            FindGameObjectWithName("Player")->transform;

    while(!token.stop_requested()) {
        if (currentCharacters < maxCharacters) {
            random = RandomnessManager::GetInstance()->GetInt(1, 10);

            if (glm::distance(playerTransform->GetLocalPosition(), jazzHoodCenter) < JAZZ_HOOD_DISTANCE)
                jazzManSpawnRate = JAZZ_MAN_INCREASED_SPAWN_RATE;
            else
                jazzManSpawnRate = JAZZ_MAN_DEFAULT_SPAWN_RATE;

            if (random <= jazzManSpawnRate)
                ch = Prefab::Instantiate<JazzTrumpet>();
            else
                ch = Prefab::Instantiate<Default>();

            if (playerIsPlaying)
                ch->GetComponent<CharacterLogic>()->SetPlayerPlayingStatus(true);

            ++currentCharacters;
        }

        std::this_thread::sleep_for(delay);
    }
}
