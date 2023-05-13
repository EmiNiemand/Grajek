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

    if(type == AnimationBehaviour::Resetable) checkpointIndex = checkpoints.size();

    CalcValueDelta();
}

void UIAnimator::Update() {
#ifdef DEBUG
    ZoneScopedNC("UI Animator", 0x800080);
#endif
    if(paused) return;

    // Switch checkpoints if current one finished
    // ------------------------------------------
    if(counter > checkpoint.duration) {
        checkpointIndex++;
        counter = 0;

        // Finish, loop or suspend animation
        // ---------------------------------
        if(checkpointIndex >= checkpoints.size()) {
            switch(type) {
                case Resetable:
                    return;
                case OneTime:
                    GameObject::Destroy(parent);
                    return;
                case Looping:
                    checkpointIndex = 0;
                    break;
            }
        }
        checkpoint = checkpoints[checkpointIndex];
        CalcValueDelta();
    }

    switch (checkpoint.property) {
        case Position:
            image->SetPosition(image->GetParent()->transform->GetLocalPosition().x
                                        + valueDelta.x * GloomEngine::GetInstance()->deltaTime,
                               image->GetParent()->transform->GetLocalPosition().y
                                        + valueDelta.y * GloomEngine::GetInstance()->deltaTime);
            break;
        case Rotation:
            image->SetRotation(image->GetParent()->transform->GetLocalRotation().z
                                     + valueDelta.z * GloomEngine::GetInstance()->deltaTime);
            break;
        case Scale:
            image->SetScale(image->GetParent()->transform->GetLocalScale().x
                            + valueDelta.x * GloomEngine::GetInstance()->deltaTime);
            break;
        case Color:
            image->SetColor(image->GetColor() + valueDelta * GloomEngine::GetInstance()->deltaTime);
            break;
        case Alpha:
            image->SetAlpha(image->GetAlpha() + valueDelta.x * GloomEngine::GetInstance()->deltaTime);
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
}

void UIAnimator::CalcValueDelta() {
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
            break;
        case Color:
            valueDelta = (checkpoint.value
                          - image->GetColor())
                         / checkpoint.duration;
            break;
        case Alpha:
            valueDelta = (checkpoint.value
                          - image->GetAlpha())
                         / checkpoint.duration;
            break;
    }
}
