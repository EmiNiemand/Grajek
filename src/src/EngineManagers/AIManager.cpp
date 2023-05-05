//
// Created by Adrian on 05.04.2023.
//

#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/Prefabs/CharacterDefault.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterLogic.h"
#include "spdlog/spdlog.h"

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

    int random;

    for (int i = 0; i < min; i++) {
        random = RandomnessManager::GetInstance()->GetInt(0, 0);

        switch (random) {
            case 0:
                auto ch = Prefab::Instantiate<CharacterDefault>();
                currentCharactersLogics.insert({ch->GetId(), ch->GetComponent<CharacterLogic>()});
                break;

        }
    }

    characterSpawner = std::jthread(SpawnCharacters, std::ref(mutex), playerIsPlaying, maxCharacters, spawnDelay,
                                    &currentCharactersLogics);
}

void AIManager::Free() {
    characterSpawner.request_stop();
    characterSpawner.join();
    currentCharactersLogics.clear();
}

void AIManager::NotifyPlayerStartsPlaying(const InstrumentName &ins, const MusicGenre &gen) {
    mutex.lock();

    playerIsPlaying = true;

    for (auto&& ch : currentCharactersLogics) {
        ch.second->SetPlayerInstrumentAndGenre(ins, gen);
        ch.second->SetPlayerPlayingStatus(true);
    }

    mutex.unlock();
}

void AIManager::NotifyPlayerStopsPlaying() {
    mutex.lock();

    playerIsPlaying = false;

    for (auto&& ch : currentCharactersLogics) {
        ch.second->SetPlayerPlayingStatus(false);
    }

    mutex.unlock();
}

void AIManager::NotifyPlayerPlayedPattern(const std::shared_ptr<MusicPattern>& pat) {
    mutex.lock();

    for (auto&& ch : currentCharactersLogics) {
        ch.second->SetPlayerPattern(pat);
    }

    mutex.unlock();
}

void AIManager::SpawnCharacters(const std::stop_token& token, std::mutex& mutex, const bool& playerIsPlaying,
                                const int& maxCharacters, const int& spawnDelay,
                                std::unordered_map<int, std::shared_ptr<CharacterLogic>>* currentCharactersLogics) {

    auto delay = std::chrono::milliseconds(spawnDelay);
    int random, charactersAmount = (int)currentCharactersLogics->size();
    std::shared_ptr<GameObject> character;

    while(!token.stop_requested()) {
        if (charactersAmount < maxCharacters) {
            random = RandomnessManager::GetInstance()->GetInt(0, 0);

            mutex.lock();

            switch (random) {
                case 0:
                    character = Prefab::Instantiate<CharacterDefault>();
                    currentCharactersLogics->insert({character->GetId(), character->GetComponent<CharacterLogic>()});
                    break;

            }

            if (playerIsPlaying)
                character->GetComponent<CharacterLogic>()->SetPlayerPlayingStatus(true);

            mutex.unlock();

            ++charactersAmount;
        }

        std::this_thread::sleep_for(delay);
    }
}