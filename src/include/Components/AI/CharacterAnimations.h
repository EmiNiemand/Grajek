//
// Created by szymo on 23/05/2023.
//

#ifndef GLOOMENGINE_CHARACTERANIMATIONS_H
#define GLOOMENGINE_CHARACTERANIMATIONS_H

#include "Components/AI/CharacterStates.h"

#include <memory>

class Animator;

class CharacterAnimations {
private:
    AI_ANIMATIONSTATE currentState = Idle;

    std::weak_ptr<Animator> animator;

public:
    explicit CharacterAnimations(const std::weak_ptr<Animator> &animator);
    virtual ~CharacterAnimations();

    void SetNewState(AI_ANIMATIONSTATE state);

private:
    void SetNewAnimation();
};


#endif //GLOOMENGINE_CHARACTERANIMATIONS_H
