//
// Created by Adrian on 01.05.2023.
//

#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/UI/Indicator.h"
#include "Components/Renderers/Animator.h"
#include "Components/AI/CharacterAnimations.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterLogic::CharacterLogic(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterLogic::~CharacterLogic() = default;

void CharacterLogic::Update() {
    if (characterMovement->rigidbody->velocity == glm::vec3(0)) {
        characterAnimations->SetNewState(AI_ANIMATIONSTATE::Idle);
    }
    else {
        characterAnimations->SetNewState(AI_ANIMATIONSTATE::Walking);
    }

    Component::Update();
}

void CharacterLogic::OnCreate() {
    characterMovement = parent->GetComponent<CharacterMovement>();
    characterIndicator = parent->GetComponent<Indicator>();
    auto animatorObject = GameObject::Instantiate("Animator", parent);
    animatorObject->transform->SetLocalRotation({0, 180, 0});
    characterAnimator = animatorObject->AddComponent<Animator>();
    characterAnimator->LoadAnimationModel("JazzMan001/JazzMan001.dae");
    characterAnimator->SetAnimation("AnimsNew/Idle1.dae");
    characterAnimations = std::make_shared<CharacterAnimations>(characterAnimator);
    minSatisfaction = RandomnessManager::GetInstance()->GetFloat(30, 50);
    Component::OnCreate();
}

void CharacterLogic::OnDestroy() {
    characterAnimations.reset();
    characterMovement = nullptr;
    characterIndicator = nullptr;
    characterAnimator = nullptr;
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
    Component::OnDestroy();
}

void CharacterLogic::SetPathToPlayer() {
    currentState = RunningToPlayer;
    characterIndicator->Indicate();
    characterMovement->SetNewPath(currentState);
}

void CharacterLogic::ReturnToPreviousPath() {
    currentState = WalkingOnPath;
    characterMovement->SetNewPath(currentState);
}

void CharacterLogic::SetPlayerInstrumentAndGenre(const InstrumentName& ins, const MusicGenre& gen) {
    playerInstrumentName = ins;
    playerGenre = gen;
}

void CharacterLogic::SetPlayerPattern(const std::shared_ptr<MusicPattern>& pat) {
    playerPattern = pat;

    if (std::find(favPatterns.begin(), favPatterns.end(), playerPattern) != favPatterns.end())
        currentSatisfaction += 15;
    else
        currentSatisfaction -= 5;

    currentSatisfaction = std::clamp(currentSatisfaction, 0.0f, 100.0f);

    if (currentSatisfaction >= minSatisfaction && currentState == WalkingOnPath) {
        SetPathToPlayer();
    } else if (currentSatisfaction < minSatisfaction && currentState == RunningToPlayer) {
        ReturnToPreviousPath();
    }
}

void CharacterLogic::SetPlayerPlayingStatus(bool state) {
    if (state) {
        currentState = AlertedByPlayer;
        characterAnimations->SetNewState(AI_ANIMATIONSTATE::Running);
        CalculateSatisfaction();
    } else {
        ReturnToPreviousPath();
    }
}

void CharacterLogic::CalculateSatisfaction() {
    currentSatisfaction = 100;

    if (std::find(favGenres.begin(), favGenres.end(), playerGenre) != favGenres.end())
        currentSatisfaction += 30;

    if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), playerInstrumentName)
    != favInstrumentsNames.end())
        currentSatisfaction += 20;

    if (currentSatisfaction > minSatisfaction)
        SetPathToPlayer();
}

const float CharacterLogic::GetCurrentSatisfaction() const {
    return currentSatisfaction;
}
