#include "Components/Animations/UIAnimator.h"
#include <utility>
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GloomEngine.h"
#include "Components/UI/Image.h"

UIAnimator::UIAnimator(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void UIAnimator::Setup(std::shared_ptr<Image> animatedImage,
                       std::vector<AnimationCheckpoint> animation,
                       bool looping) {
    image = std::move(animatedImage);
    checkpoints = std::move(animation);
    checkpoint = checkpoints[0];
    loopingAnimation = looping;
    counter = 0;

    switch(checkpoint.property) {
        case Position:
            valueDelta = (checkpoint.value
                          - image->GetParent()->transform->GetLocalPosition())
                          / checkpoint.duration;
            break;
        case Rotation:
            valueDelta = (checkpoint.value
                          - image->GetParent()->transform->GetLocalRotation())
                          / checkpoint.duration;
            break;
        case Scale:
            valueDelta = (checkpoint.value
                          - image->GetParent()->transform->GetLocalScale())
                          / checkpoint.duration;
    }
}

void UIAnimator::Update() {
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
        switch(checkpoint.property) {
            case Position:
                valueDelta = (checkpoint.value
                              - image->GetParent()->transform->GetLocalPosition())
                              / checkpoint.duration;
                break;
            case Rotation:
                valueDelta = (checkpoint.value
                              - image->GetParent()->transform->GetLocalRotation())
                              / checkpoint.duration;
                break;
            case Scale:
                valueDelta = (checkpoint.value
                              - image->GetParent()->transform->GetLocalScale())
                              / checkpoint.duration;
        }
    }

    switch (checkpoint.property) {
        case Position:
            image->SetPosition((int)(image->GetParent()->transform->GetLocalPosition().x
                                     + valueDelta.x * GloomEngine::GetInstance()->deltaTime),
                               (int)(image->GetParent()->transform->GetLocalPosition().y
                                     + valueDelta.y * GloomEngine::GetInstance()->deltaTime));
            break;
        case Rotation:
            image->SetRotation(image->GetParent()->transform->GetLocalRotation().z
                                     + valueDelta.z * GloomEngine::GetInstance()->deltaTime);
            break;
        case Scale:
            image->SetScale(image->GetParent()->transform->GetLocalScale().x
                            + valueDelta.x * GloomEngine::GetInstance()->deltaTime);
            break;
    }

    counter += GloomEngine::GetInstance()->deltaTime;
}