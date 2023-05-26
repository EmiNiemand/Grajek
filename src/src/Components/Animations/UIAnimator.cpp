#include "Components/Animations/UIAnimator.h"
#include <utility>
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GloomEngine.h"
#include "Components/UI/Image.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

UIAnimator::UIAnimator(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void UIAnimator::Setup(std::shared_ptr<Image> animatedImage,
                       std::vector<AnimationCheckpoint> animation,
                       AnimationBehaviour behaviour) {
    image = std::move(animatedImage);
    checkpoints = std::move(animation);
    checkpoint = checkpoints[0];
    type = behaviour;
    counter = 0;

    if(type == AnimationBehaviour::Resetable) {
        counter = checkpoint.duration+1;
        checkpointIndex = checkpoints.size();
    }

    CalcValueDelta();
}

void UIAnimator::Update() {
#ifdef DEBUG
    ZoneScopedNC("UI Animator", 0x800080);
#endif
    if(paused) return;
    if(type == AnimationBehaviour::Resetable && checkpointIndex >= checkpoints.size()) return;

    // Switch checkpoints if current one finished
    // ------------------------------------------
    if(counter > checkpoint.duration) {
        checkpointIndex++;

        // Snap property value to checkpoint value
        switch (checkpoint.property) {
            case AnimatedProperty::Position:
                image->SetPosition(checkpoint.value.x, checkpoint.value.y);
                break;
            case AnimatedProperty::Rotation:
                image->SetRotation(checkpoint.value.z);
                break;
            case AnimatedProperty::Scale:
                image->SetScale(checkpoint.value.x);
                break;
            case AnimatedProperty::Color:
                image->SetColor(checkpoint.value);
                break;
            case AnimatedProperty::Alpha:
                image->SetAlpha(checkpoint.value.x);
                break;
        }

        // Finish, loop or suspend animation
        // ---------------------------------
        if(checkpointIndex >= checkpoints.size()) {
            auto imageTransform = image->GetParent()->transform;
            switch(type) {
                case AnimationBehaviour::Resetable:
                    return;
                case AnimationBehaviour::OneTime:
                    GameObject::Destroy(parent);
                    return;
                case AnimationBehaviour::Looping:
                    checkpointIndex = 0;
                    break;
            }
        }
        counter = 0;
        checkpoint = checkpoints[checkpointIndex];
        CalcValueDelta();
    }

    float deltaTime = GloomEngine::GetInstance()->deltaTime;
    auto imageTransform = image->GetParent()->transform;
    switch (checkpoint.property) {
        case AnimatedProperty::Position:
            image->SetPosition(imageTransform->GetLocalPosition().x + valueDelta.x * deltaTime,
                               imageTransform->GetLocalPosition().y + valueDelta.y * deltaTime);
            break;
        case AnimatedProperty::Rotation:
            image->SetRotation(imageTransform->GetLocalRotation().z + valueDelta.z * deltaTime);
            break;
        case AnimatedProperty::Scale:
            image->SetScale(imageTransform->GetLocalScale().x + valueDelta.x * deltaTime);
            break;
        case AnimatedProperty::Color:
            image->SetColor(image->GetColor() + valueDelta * deltaTime);
            break;
        case AnimatedProperty::Alpha:
            image->SetAlpha(image->GetAlpha() + valueDelta.x * deltaTime);
            break;
    }

    counter += GloomEngine::GetInstance()->deltaTime;
    Component::Update();
}

void UIAnimator::Reset() {
    if(type != AnimationBehaviour::Resetable) return;
    checkpointIndex = 0;
    checkpoint = checkpoints[0];
    counter = 0;
    CalcValueDelta();
}

void UIAnimator::CalcValueDelta() {
    auto imageTransform = image->GetParent()->transform;
    switch(checkpoint.property) {
        case AnimatedProperty::Position:
            valueDelta = (checkpoint.value
                          - imageTransform->GetLocalPosition())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Rotation:
            valueDelta = (checkpoint.value
                          - imageTransform->GetLocalRotation())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Scale:
            valueDelta = glm::vec3(checkpoint.value
                        - imageTransform->GetLocalScale())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Color:
            valueDelta = (checkpoint.value
                          - image->GetColor())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Alpha:
            valueDelta = (checkpoint.value
                          - image->GetAlpha())
                         / checkpoint.duration;
            break;
    }
}
