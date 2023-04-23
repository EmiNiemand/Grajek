//
// Created by MasterKtos on 23.04.2023.
//

#include "GameObjectAnimator.h"

#include <utility>
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GloomEngine.h"
#include "Utilities.h"

GameObjectAnimator::GameObjectAnimator(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void GameObjectAnimator::Setup(std::shared_ptr<Transform> animatedTransform,
                               std::vector<AnimationCheckpoint> animation,
                               bool looping) {
    transform = std::move(animatedTransform);
    checkpoints = std::move(animation);
    checkpoint = checkpoints[0];
    loopingAnimation = looping;
    counter = 0;
}

void GameObjectAnimator::Update() {
    Component::Update();

    // Switch checkpoints if current one finished
    // ------------------------------------------
    if(counter > checkpoint.duration) {
        checkpointIndex++;
        counter = 0;

        // Finish or loop animation
        // ------------------------
        if(checkpointIndex >= checkpoints.size()) {
            if(loopingAnimation)
                checkpointIndex = 0;
            else {
                GameObject::Destroy(parent);
                return;
            }
        }
        checkpoint = checkpoints[checkpointIndex];
    }

    switch (checkpoint.property) {
        case Position:
            transform->SetLocalPosition(
                    Utilities::Lerp(transform->GetLocalPosition(),
                            checkpoint.value, 0.3f));
            break;
        case Rotation:
            transform->SetLocalRotation(
                    Utilities::Lerp(transform->GetLocalRotation(),
                                    checkpoint.value, 0.3f));
            break;
        case Scale:
            transform->SetLocalScale(
                    Utilities::Lerp(transform->GetLocalScale(),
                                    checkpoint.value, 0.3f));
            break;
    }

    counter += GloomEngine::GetInstance()->deltaTime;
    spdlog::info("Counter: " + std::to_string(counter) + ";\t transform.y: " + std::to_string(transform->GetLocalPosition().y));
}
