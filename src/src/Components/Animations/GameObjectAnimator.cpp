//
// Created by MasterKtos on 23.04.2023.
//

#include "Components/Animations/GameObjectAnimator.h"

#include <utility>
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GloomEngine.h"
#include "Utilities.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

GameObjectAnimator::GameObjectAnimator(const std::shared_ptr<GameObject> &parent, uint32_t id) : Component(parent, id) {}

void GameObjectAnimator::Setup(std::shared_ptr<Transform> animatedTransform,
                               std::vector<AnimationCheckpoint> animation,
                               bool looping, bool absoluteValues) {
    transform = std::move(animatedTransform);
    checkpoints = std::move(animation);
    checkpoint = checkpoints[0];
    loopingAnimation = looping;
    useAbsoluteValues = absoluteValues;
    counter = 0;

    switch(checkpoint.property) {
        case Position:
            valueDelta = (checkpoint.value
                          - transform->GetLocalPosition()
                            * (float)useAbsoluteValues)
                                /checkpoint.duration;
            break;
        case Rotation:
            valueDelta = (checkpoint.value
                          - transform->GetLocalRotation()
                            * (float)useAbsoluteValues)
                                /checkpoint.duration;
            break;
        case Scale:
            valueDelta = (checkpoint.value * transform->GetLocalScale()
                        - transform->GetLocalScale())
                                /checkpoint.duration;
    }

}

void GameObjectAnimator::Update() {
#ifdef DEBUG
    ZoneScopedNC("Game Object Animator", 0x800080);
#endif
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
        switch(checkpoint.property) {
            case Position:
                valueDelta = (checkpoint.value
                              - transform->GetLocalPosition()
                                * (float)useAbsoluteValues)
                             /checkpoint.duration;
                break;
            case Rotation:
                valueDelta = (checkpoint.value
                              - transform->GetLocalRotation()
                                * (float)useAbsoluteValues)
                             /checkpoint.duration;
                break;
            case Scale:
                valueDelta = (checkpoint.value * transform->GetLocalScale()
                              - transform->GetLocalScale())
                             /checkpoint.duration;
                break;
        }
    }

    switch (checkpoint.property) {
        case Position:
            transform->SetLocalPosition(
                    transform->GetLocalPosition()
                    + valueDelta * GloomEngine::GetInstance()->deltaTime);
            break;
        case Rotation:
            transform->SetLocalRotation(
                    transform->GetLocalRotation()
                    + valueDelta * GloomEngine::GetInstance()->deltaTime);
            break;
        case Scale:
            transform->SetLocalScale(
                    transform->GetLocalScale()
                    + valueDelta * GloomEngine::GetInstance()->deltaTime);
            break;
    }

    counter += GloomEngine::GetInstance()->deltaTime;
    Component::Update();
}
