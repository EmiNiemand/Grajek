//
// Created by Adrian on 05.04.2023.
//

#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
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

    characterSpawner = std::jthread(SpawnCharacters, std::ref(mutex), playerIsPlaying, maxCharacters, spawnDelay,
                                    &charactersLogics);
}

void AIManager::Free() {
    if (characterSpawner.joinable()) {
        characterSpawner.request_stop();
        characterSpawner.join();
    }
    charactersLogics.clear();
}

void AIManager::NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    mutex.lock();

    playerIsPlaying = true;

    for (auto&& ch : charactersLogics) {
        ch.second->SetPlayerInstrumentAndGenre(ins, gen);
        ch.second->SetPlayerPlayingStatus(true);
    }

    mutex.unlock();
}

void AIManager::NotifyPlayerStopsPlaying() {
    mutex.lock();

    playerIsPlaying = false;

    for (auto&& ch : charactersLogics) {
        ch.second->SetPlayerPlayingStatus(false);
    }

    mutex.unlock();
}

void AIManager::NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    mutex.lock();

    for (auto&& ch : charactersLogics) {
        ch.second->SetPlayerPattern(pat);
    }

    mutex.unlock();
}

const float AIManager::GetCombinedPlayerSatisfaction() {
    float satisfaction = 0.0f;

    mutex.lock();

    for (auto&& ch : charactersLogics) {
        satisfaction += ch.second->GetPlayerSatisfaction();
    }

    satisfaction /= (float)charactersLogics.size();

    mutex.unlock();

    return satisfaction;
}

void AIManager::NotifyEnemyStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    mutex.lock();

    playerIsPlaying = true;

    for (auto&& ch : charactersLogics) {
        ch.second->SetEnemyInstrumentAndGenre(ins, gen);
        ch.second->SetEnemyPlayingStatus(true);
    }

    mutex.unlock();
}

void AIManager::NotifyEnemyStopsPlaying() {
    mutex.lock();

    playerIsPlaying = false;

    for (auto&& ch : charactersLogics) {
        ch.second->SetEnemyPlayingStatus(false);
    }

    mutex.unlock();
}

void AIManager::NotifyEnemyPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    mutex.lock();

    for (auto&& ch : charactersLogics) {
        ch.second->SetEnemyPattern(pat);
    }

    mutex.unlock();
}

const float AIManager::GetCombinedEnemySatisfaction() {
    float satisfaction = 0.0f;

    mutex.lock();

    for (auto&& ch : charactersLogics) {
        satisfaction += ch.second->GetEnemySatisfaction();
    }

    satisfaction /= (float)charactersLogics.size();

    mutex.unlock();

    return satisfaction;
}

void AIManager::RemoveCharacterLogic(const int& componentId) {
    if (charactersLogics.contains(componentId))
        charactersLogics.erase(componentId);
}

void AIManager::RemoveCharacterMovement(const int& componentId) {
    if (charactersMovements.contains(componentId))
        charactersMovements.erase(componentId);
}

void AIManager::SpawnCharacters(const std::stop_token& token, std::mutex& mutex, const bool& playerIsPlaying,
                                const int& maxCharacters, const int& spawnDelay,
                                std::unordered_map<int, std::shared_ptr<CharacterLogic>>* currentCharactersLogics) {

    auto delay = std::chrono::milliseconds(spawnDelay);
    int random, charactersAmount = (int)currentCharactersLogics->size();
    std::shared_ptr<GameObject> ch;

    while(!token.stop_requested()) {
        if (charactersAmount < maxCharacters) {
            random = RandomnessManager::GetInstance()->GetInt(0, 2);

            mutex.lock();

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

            mutex.unlock();

            ++charactersAmount;
        }

        std::this_thread::sleep_for(delay);
    }
}
