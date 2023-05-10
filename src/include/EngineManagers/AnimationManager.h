//
// Created by szymo on 09/05/2023.
//

#ifndef GLOOMENGINE_ANIMATIONMANAGER_H
#define GLOOMENGINE_ANIMATIONMANAGER_H

#include <thread>
#include <vector>

class Animator;

class AnimationManager {
private:
    inline static AnimationManager* animationManager;

    inline static unsigned int maxNumberOfThreads = std::thread::hardware_concurrency() / 2;
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<Animator>> buffer;

public:
    AnimationManager(AnimationManager &other) = delete;
    void operator=(const AnimationManager&) = delete;
    virtual ~AnimationManager();

    static AnimationManager* GetInstance();

    void AddToBuffer(const std::shared_ptr<Animator>& animator);
    void UpdateAnimations();

private:
    explicit AnimationManager();

    void ConcurrenceCalculation(std::vector<std::shared_ptr<Animator>> animators, float deltaTime);
};


#endif //GLOOMENGINE_ANIMATIONMANAGER_H
