//
// Created by Adrian on 03.05.2023.
//

#include "EngineManagers/RandomnessManager.h"
#include <random>
#include <string>

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

RandomnessManager::RandomnessManager() = default;

RandomnessManager::~RandomnessManager() {
    delete randomnessManager;
}

RandomnessManager* RandomnessManager::GetInstance() {
    return (randomnessManager == nullptr) ? randomnessManager = new RandomnessManager() : randomnessManager;
}

void RandomnessManager::InitializeRandomEngine() {
#ifdef DEBUG
    ZoneScopedNC("RandomnessManager", 0xDC143C);
#endif
    // NOTE: sometimes, depending on implementation and device, random_device can produce deterministic
    // integers, i.e. same value even after reinitialization
    std::random_device rd;
    std::seed_seq seed {rd(), rd()};
    randomEngine.seed(seed);
}

void RandomnessManager::Free() { }

const int RandomnessManager::GetInt(const int& min, const int& max) {
    std::uniform_int_distribution<int> uniform_dist(min, max);
    return uniform_dist(randomEngine);
}

const float RandomnessManager::GetFloat(const float& min, const float& max) {
    std::uniform_real_distribution<float> uniform_dist(min, max);
    return uniform_dist(randomEngine);
}
