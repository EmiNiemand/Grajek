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

const float AIManager::GetCombinedSatisfaction() {
    float satisfaction = 0.0f;

    mutex.lock();

    for (auto&& ch : charactersLogics) {
        satisfaction += ch.second->GetCurrentSatisfaction();
    }

    satisfaction /= (float)charactersLogics.size();

    mutex.unlock();

    return satisfaction;
}

void AIManager::RemoveCharacterLogic(const int& componentId) {
    if (charactersLogics.contains(componentId))
        charactersLogics.erase(componentId);
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

void AIManager::RemoveBoxCollider(const std::shared_ptr<BoxCollider>& ptr) const {
    glm::mat4 model = ptr->GetModelMatrix();

    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(ptr->GetParent()->globalRotation.x),
                                             glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(ptr->GetParent()->globalRotation.y),
                                             glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(ptr->GetParent()->globalRotation.z),
                                             glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    glm::vec4 pos = model * glm::vec4(0, 0, 0, 1);

    glm::vec4 xVec = (rotationMatrix * glm::vec4(1, 0, 0, 1)) * ptr->GetSize().x *
                     ptr->GetParent()->transform->GetGlobalScale().x;
    glm::vec4 zVec = (rotationMatrix * glm::vec4(0, 0, 1, 1)) * ptr->GetSize().z *
                     ptr->GetParent()->transform->GetGlobalScale().z;

    auto xVector = glm::vec2(xVec.x, xVec.z);
    auto zVector = glm::vec2(zVec.x, zVec.z);

    glm::ivec2 points[4] = {
            glm::ivec2((glm::vec2(pos.x, pos.z) + (xVector + zVector)) / aiCellSize),
            glm::ivec2((glm::vec2(pos.x, pos.z) + (xVector - zVector)) / aiCellSize),
            glm::ivec2((glm::vec2(pos.x, pos.z) + (-xVector + zVector)) / aiCellSize),
            glm::ivec2((glm::vec2(pos.x, pos.z) + (-xVector - zVector)) / aiCellSize)
    };

    if (points[0] == points[1] &&
        points[1] == points[2] &&
        points[2] == points[3]) {
        int x = points[0].x;
        int y = points[0].y;

        AIManager::GetInstance()->aiGrid[x + AI_GRID_SIZE / 2][y + AI_GRID_SIZE / 2] = true;
        return;
    }

    int minX = points[0].x;
    int minY = points[0].y;
    int maxX = points[0].x;
    int maxY = points[0].y;

    for (int i = 1; i < 4; i++) {
        if (minX > points[i].x) minX = points[i].x;
        if (minY > points[i].y) minY = points[i].y;
        if (maxX < points[i].x) maxX = points[i].x;
        if (maxY < points[i].y) maxY = points[i].y;
    }

    for (int x = minX - 1; x <= maxX + 1; ++x) {
        for (int y = minY - 1; y <= maxY + 1; ++y) {
            AIManager::GetInstance()->aiGrid[x + AI_GRID_SIZE / 2][y + AI_GRID_SIZE / 2] = true;
        }
    }
}
