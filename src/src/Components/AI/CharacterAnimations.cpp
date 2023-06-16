//
// Created by szymo on 23/05/2023.
//

#include "Components/AI/CharacterAnimations.h"
#include "Components/Renderers/Animator.h"

CharacterAnimations::CharacterAnimations(const std::weak_ptr<Animator> &animator) : animator(animator) {
    animator.lock()->SetAnimation("CrowdAnimations/Idle1.dae");
}

CharacterAnimations::~CharacterAnimations() {
    animator.reset();
}

void CharacterAnimations::SetNewState(AI_ANIMATION_STATE state) {
    if (state != currentState) {
        currentState = state;
        SetNewAnimation();
    }
}

void CharacterAnimations::SetNewAnimation() {
    // TODO: implement logic for all animations
    switch(currentState) {
        case AI_ANIMATION_STATE::Idle:
            animator.lock()->SetAnimation("CrowdAnimations/Idle1.dae");
            break;
        case AI_ANIMATION_STATE::Walking:
            animator.lock()->SetAnimation("CrowdAnimations/Walk.dae");
            break;
        case AI_ANIMATION_STATE::Running:
            animator.lock()->SetAnimation("CrowdAnimations/Walk.dae");
            animator.lock()->speed = 3;
            break;
        case AI_ANIMATION_STATE::Cheering:
            animator.lock()->SetAnimation("CrowdAnimations/Happy.dae");
            break;
        case AI_ANIMATION_STATE::Booing:
            animator.lock()->SetAnimation("CrowdAnimations/Angry.dae");
            break;
    }
}
