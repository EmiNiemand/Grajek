#include "Components/Animations/UIAnimator.h"
#include <utility>
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GloomEngine.h"
#include "Components/UI/Image.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

UIAnimator::UIAnimator(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void UIAnimator::Setup(std::shared_ptr<UIComponent> animatedUIComponent,
                       std::vector<AnimationCheckpoint> animation,
                       AnimationBehaviour behaviour) {
    uiComponent = std::move(animatedUIComponent);
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
                uiComponent->SetPosition(checkpoint.value.x, checkpoint.value.y);
                break;
            case AnimatedProperty::Rotation:
                uiComponent->SetRotation(checkpoint.value.z);
                break;
            case AnimatedProperty::Scale:
                uiComponent->SetScale(checkpoint.value.x);
                break;
            case AnimatedProperty::Color:
                uiComponent->SetColor(checkpoint.value);
                break;
            case AnimatedProperty::Alpha:
                uiComponent->SetAlpha(checkpoint.value.x);
                break;
        }

        // Finish, loop or suspend animation
        // ---------------------------------
        if(checkpointIndex >= checkpoints.size()) {
            auto imageTransform = uiComponent->GetParent()->transform;
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
    auto uiComponentTransform = uiComponent->GetParent()->transform;
    switch (checkpoint.property) {
        case AnimatedProperty::Position:
            uiComponent->SetPosition(uiComponentTransform->GetLocalPosition().x + valueDelta.x * deltaTime,
                                     uiComponentTransform->GetLocalPosition().y + valueDelta.y * deltaTime);
            break;
        case AnimatedProperty::Rotation:
            uiComponent->SetRotation(uiComponentTransform->GetLocalRotation().z + valueDelta.z * deltaTime);
            break;
        case AnimatedProperty::Scale:
            uiComponent->SetScale(uiComponentTransform->GetLocalScale().x + valueDelta.x * deltaTime);
            break;
        case AnimatedProperty::Color:
            uiComponent->SetColor(uiComponent->GetColor() + valueDelta * deltaTime);
            break;
        case AnimatedProperty::Alpha:
            uiComponent->SetAlpha(uiComponent->GetAlpha() + valueDelta.x * deltaTime);
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
    auto uiComponentTransform = uiComponent->GetParent()->transform;
    switch(checkpoint.property) {
        case AnimatedProperty::Position:
            valueDelta = (checkpoint.value
                          - uiComponentTransform->GetLocalPosition())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Rotation:
            valueDelta = (checkpoint.value
                          - uiComponentTransform->GetLocalRotation())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Scale:
            valueDelta = glm::vec3(checkpoint.value
                        - uiComponentTransform->GetLocalScale())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Color:
            valueDelta = (checkpoint.value
                          - uiComponent->GetColor())
                         / checkpoint.duration;
            break;
        case AnimatedProperty::Alpha:
            valueDelta = (checkpoint.value
                          - uiComponent->GetAlpha())
                         / checkpoint.duration;
            break;
    }
}

void UIAnimator::OnDestroy() {
    uiComponent.reset();
    Component::OnDestroy();
}
