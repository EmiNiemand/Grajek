//
// Created by szymo on 23/05/2023.
//

#include "Components/AI/CharacterAnimations.h"
#include "Components/Renderers/Animator.h"

CharacterAnimations::CharacterAnimations(const std::shared_ptr<Animator> &animator) : animator(animator) {
    animator->SetAnimation("AnimsNew/Idle1.dae");
}

CharacterAnimations::~CharacterAnimations() {
    animator.reset();
}

void CharacterAnimations::SetNewState(AI_ANIMATIONSTATE state) {
    if (state != currentState) {
        currentState = state;
        SetNewAnimation();
    }
}

void CharacterAnimations::SetNewAnimation() {
    // TODO: implement logic for all animations
    switch(currentState) {
        case AI_ANIMATIONSTATE::Idle:
            animator->SetAnimation("AnimsNew/Idle1.dae");
            break;
        case AI_ANIMATIONSTATE::Walking:
            animator->SetAnimation("AnimsNew/Walk.dae");
            break;
        case AI_ANIMATIONSTATE::Running:
            animator->SetAnimation("AnimsNew/Walk.dae");
            animator->speed = 3;
            break;
        case AI_ANIMATIONSTATE::Cheering:
            animator->SetAnimation("AnimsNew/Happy.dae");
            break;
        case AI_ANIMATIONSTATE::Booing:
            animator->SetAnimation("AnimsNew/Angry.dae");
            break;
    }
}
