//
// Created by Adrian on 05.04.2023.
//

#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterLogic.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/Default.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/RockDrums.h"
#include "GameObjectsAndPrefabs/Prefabs/Characters/JazzClap.h"
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
    std::shared_ptr<GameObject> ch;

    for (int i = 0; i < min; i++) {
        random = RandomnessManager::GetInstance()->GetInt(0, 2);

        switch (random) {
            case 0:
                ch = Prefab::Instantiate<RockDrums>();
                currentCharactersLogics.insert({ch->GetId(), ch->GetComponent<CharacterLogic>()});
                break;
            case 1:
                ch = Prefab::Instantiate<Default>();
                currentCharactersLogics.insert({ch->GetId(), ch->GetComponent<CharacterLogic>()});
                break;
            case 2:
                ch = Prefab::Instantiate<JazzClap>();
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
    std::shared_ptr<GameObject> ch;

    while(!token.stop_requested()) {
        if (charactersAmount < maxCharacters) {
            random = RandomnessManager::GetInstance()->GetInt(0, 0);

            mutex.lock();

            switch (random) {
                case 0:
                    ch = Prefab::Instantiate<RockDrums>();
                    currentCharactersLogics->insert({ch->GetId(), ch->GetComponent<CharacterLogic>()});
                    break;
                case 1:
                    ch = Prefab::Instantiate<Default>();
                    currentCharactersLogics->insert({ch->GetId(), ch->GetComponent<CharacterLogic>()});
                    break;
                case 2:
                    ch = Prefab::Instantiate<JazzClap>();
                    currentCharactersLogics->insert({ch->GetId(), ch->GetComponent<CharacterLogic>()});
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

const float AIManager::GetCombinedSatisfaction() {
    float satisfaction = 0.0f;

    mutex.lock();

    for (auto&& ch : currentCharactersLogics) {
        satisfaction += ch.second->GetCurrentSatisfaction();
    }

    satisfaction /= (float)currentCharactersLogics.size();

    mutex.unlock();

    return satisfaction;
}
