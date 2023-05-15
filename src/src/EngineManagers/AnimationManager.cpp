#include "EngineManagers/AnimationManager.h"
#include "GloomEngine.h"
#include "Components/Renderers/Animator.h"

AnimationManager::AnimationManager() = default;

AnimationManager::~AnimationManager() = default;

AnimationManager *AnimationManager::GetInstance() {
    if (animationManager == nullptr) {
        animationManager = new AnimationManager();
    }
    return animationManager;
}

void AnimationManager::AddToBuffer(const std::shared_ptr<Animator>& animator) {
    buffer[bufferIterator] = animator;
    ++bufferIterator;
}

void AnimationManager::UpdateAnimations() {
    auto chunk = (unsigned int)std::ceil((float)bufferIterator / (float)(maxNumberOfThreads + 1));

    std::vector<std::shared_ptr<Animator>> animators;

    const int sizeForThreads = (int)bufferIterator - chunk;

    for (int i = 0; i < sizeForThreads; ++i) {
        animators.push_back(buffer[i]);
        if (animators.size() % chunk == 0) {
            threads.emplace_back(&AnimationManager::ConcurrenceCalculation, animationManager, std::move(animators), GloomEngine::GetInstance()->deltaTime);
            animators.clear();
        }
    }

    for (int i = sizeForThreads; i < bufferIterator; ++i) {
        animators.push_back(buffer[i]);
    }

    ConcurrenceCalculation(std::move(animators), GloomEngine::GetInstance()->deltaTime);
    animators.clear();


    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    ClearBuffer();
}

void AnimationManager::ConcurrenceCalculation(std::vector<std::shared_ptr<Animator>> animators, float deltaTime) {
    for (int i = 0; i < animators.size(); ++i) {
        animators[i]->UpdateAnimation(deltaTime);
    }
}

void AnimationManager::ClearBuffer() {
    for (int i = 0; i < bufferIterator; ++i) {
        buffer[i] = nullptr;
    }
    bufferIterator = 0;
}

