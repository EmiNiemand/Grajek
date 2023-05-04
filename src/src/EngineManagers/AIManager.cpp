//
// Created by Adrian on 05.04.2023.
//

#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterLogic.h"
#include "spdlog/spdlog.h"
#include <string>

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

AIManager::AIManager() = default;

AIManager::~AIManager() = default;

AIManager* AIManager::GetInstance() {
    return (aiManager == nullptr) ? aiManager = new AIManager() : aiManager;
}

void AIManager::InitializeSpawner(const int& min, const int& max, const int& delay) {
#ifdef DEBUG
    ZoneScopedNC("AIManager", 0xDC143C);
#endif
    maxCharacters = max;
    spawnDelay = delay;

    charactersPrefabs = Prefab::GetCharacters();

    int random, prefabsAmount = charactersPrefabs.size();
    std::string name;

    for (int i = 0; i < min; i++) {
        random = RandomnessManager::GetInstance()->GetInt(0, prefabsAmount - 1);

        auto func = charactersPrefabs[random];
        name = "Character " + std::to_string(i);

        currentCharacters.insert({i, func(name)});
    }

    characterSpawner = std::jthread(SpawnCharacters, playerIsPlaying, maxCharacters, spawnDelay, charactersPrefabs,
                                    &currentCharacters);
}

void AIManager::Free() {
    characterSpawner.request_stop();
    characterSpawner.join();
    charactersPrefabs.clear();
    currentCharacters.clear();
}

void AIManager::NotifyPlayerStartsPlaying() {
    mutex.lock();

    playerIsPlaying = true;

    for (auto&& ch : currentCharacters) {
        ch.second->GetComponent<CharacterLogic>()->SetPlayerPlayingStatus(true);
    }

    mutex.unlock();
}

void AIManager::NotifyPlayerStopsPlaying() {
    mutex.lock();

    playerIsPlaying = false;

    for (auto&& ch : currentCharacters) {
        ch.second->GetComponent<CharacterLogic>()->SetPlayerPlayingStatus(false);
    }

    mutex.unlock();
}

void AIManager::SpawnCharacters(const std::stop_token& token, const bool& playerIsPlaying, const int& maxCharacters,
                                const int& spawnDelay,
                                const std::vector<std::shared_ptr<GameObject> (*)(std::string)>& charactersPrefabs,
                                std::unordered_map<int, std::shared_ptr<GameObject>>* currentCharacters) {

    auto delay = std::chrono::milliseconds(spawnDelay);
    int random, prefabsAmount = charactersPrefabs.size(), charactersAmount = currentCharacters->size();
    std::string name;

    while(!token.stop_requested()) {
        if (charactersAmount < maxCharacters) {
            random = RandomnessManager::GetInstance()->GetInt(0, prefabsAmount - 1);

            auto func = charactersPrefabs[random];
            name = "Character " + std::to_string(charactersAmount);

            auto ch = func(name);

            if (playerIsPlaying)
                ch->GetComponent<CharacterLogic>()->SetPlayerPlayingStatus(true);

            currentCharacters->insert({charactersAmount, ch});
            ++charactersAmount;
        }

        std::this_thread::sleep_for(delay);
    }
}