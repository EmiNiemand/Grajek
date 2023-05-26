//
// Created by Adrian on 01.05.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/Renderers/Animator.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/AI/CharacterPathfinding.h"
#include "Components/AI/CharacterAnimations.h"
#include "Components/UI/Indicator.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterLogic::CharacterLogic(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterLogic::~CharacterLogic() = default;

void CharacterLogic::Start() {
    characterMovement = parent->GetComponent<CharacterMovement>();
    characterIndicator = parent->GetComponent<Indicator>();
    auto animatorObject = GameObject::Instantiate("Animator", parent);
    animatorObject->transform->SetLocalRotation({0, 180, 0});
    auto characterAnimator = animatorObject->AddComponent<Animator>();
    characterAnimator->LoadAnimationModel("JazzMan001/JazzMan001.dae");
    characterAnimator->SetAnimation("AnimsNew/Idle1.dae");
    characterAnimations = std::make_shared<CharacterAnimations>(characterAnimator);
    minSatisfaction = RandomnessManager::GetInstance()->GetFloat(30, 50);
    Component::Start();
}

void CharacterLogic::Update() {
    if (logicState != ListeningToPlayer) {
        switch (characterMovement->GetCurrentStatus()) {
            case OnPathToPlayer:
                characterAnimations->SetNewState(Running);
                break;
            case OnPathToTarget:
                characterAnimations->SetNewState(Walking);
                break;
            case NearTargetSubPoint:
                characterAnimations->SetNewState(Idle);
                break;
            case NearPlayer:
                logicState = ListeningToPlayer;
                break;
            default:
                break;
        }
    }

    if (logicState == ListeningToPlayer) {
        if (currentSatisfaction > 70.0f) {
            characterAnimations->SetNewState(Cheering);
        } else if (currentSatisfaction < 40.0f && currentSatisfaction >= 30.0f) {
            characterAnimations->SetNewState(Booing);
        } else if (currentSatisfaction < 30.0f) {
            logicState = AlertedByPlayer;
            characterMovement->ReturnToPreviousPath();
        } else {
            characterAnimations->SetNewState(Idle);
        }
    }

    Component::Update();
}

void CharacterLogic::AIUpdate() {
    if (logicState == AlertedByPlayer) {
        CalculateSatisfaction();

        if (currentSatisfaction > minSatisfaction) {
            characterMovement->SetNewPathToPlayer();
            characterIndicator->Indicate();
            logicState = MovingToPlayer;
        }
    }

    Component::AIUpdate();
}

void CharacterLogic::OnCreate() {
    AIManager::GetInstance()->charactersLogics.insert({id, std::dynamic_pointer_cast<CharacterLogic>(shared_from_this())});
    Component::OnCreate();
}

void CharacterLogic::OnDestroy() {
    AIManager::GetInstance()->RemoveCharacterLogic(id);
    characterAnimations = nullptr;
    characterMovement = nullptr;
    characterIndicator = nullptr;
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
    Component::OnDestroy();
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
}

void CharacterLogic::SetPlayerPlayingStatus(bool isPlayerPlaying) {
    if (isPlayerPlaying) {
        logicState = AlertedByPlayer;
    } else {
        if (logicState != None) {
            characterMovement->ReturnToPreviousPath();
        }

        logicState = None;
    }
}

void CharacterLogic::CalculateSatisfaction() {
    currentSatisfaction = 100;

    if (std::find(favGenres.begin(), favGenres.end(), playerGenre) != favGenres.end())
        currentSatisfaction += 30;

    if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), playerInstrumentName)
    != favInstrumentsNames.end())
        currentSatisfaction += 20;
}

const float CharacterLogic::GetCurrentSatisfaction() const {
    return currentSatisfaction;
}
