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
    buffer.push_back(animator);
}

void AnimationManager::UpdateAnimations() {
    auto chunk = (unsigned int)std::ceil((float)buffer.size() / (float)(maxNumberOfThreads + 1));

    std::vector<std::shared_ptr<Animator>> animators;

    for (int i = 0; i < buffer.size(); i++) {
        animators.push_back(buffer[i]);
        if (animators.size() % chunk == 0) {
            threads.emplace_back(&AnimationManager::ConcurrenceCalculation, animationManager, std::move(animators), GloomEngine::GetInstance()->deltaTime);
            animators.clear();
        }
    }

    if (!animators.empty()) {
        ConcurrenceCalculation(std::move(animators), GloomEngine::GetInstance()->deltaTime);
        animators.clear();
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    buffer.clear();
}

void AnimationManager::ConcurrenceCalculation(std::vector<std::shared_ptr<Animator>> animators, float deltaTime) {
    for (int i = 0; i < animators.size(); i++) {
        animators[i]->UpdateAnimation(deltaTime);
    }
}

