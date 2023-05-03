//
// Created by Adrian on 03.05.2023.
//

#ifndef GLOOMENGINE_RANDOMNESSMANAGER_H
#define GLOOMENGINE_RANDOMNESSMANAGER_H

#include <memory>
#include <unordered_map>
#include <random>

class GloomEngine;
class GameObject;

class RandomnessManager {
    std::mt19937 randomEngine;

    inline static RandomnessManager* randomnessManager;
    explicit RandomnessManager();

public:
    RandomnessManager(RandomnessManager &other) = delete;
    void operator=(const RandomnessManager&) = delete;
    virtual ~RandomnessManager();

    static RandomnessManager* GetInstance();
    void Free();

    void InitializeRandomEngine();

    const int GetInt(const int& min, const int& max);
    const float GetFloat(const float& min, const float& max);
};

#endif //GLOOMENGINE_RANDOMNESSMANAGER_H
