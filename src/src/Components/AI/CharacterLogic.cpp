//
// Created by Adrian on 01.05.2023.
//

#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "Components/Renderers/Animator.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
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
    characterAnimator->LoadAnimationModel(modelPath);
    characterAnimator->SetAnimation("AnimsNew/Idle3.dae");
    characterAnimations = std::make_shared<CharacterAnimations>(characterAnimator);
    minSatisfaction = RandomnessManager::GetInstance()->GetFloat(35, 50);
    lowerSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(25, 40);
    middleSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(50, 65);
    upperSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(75, 85);
    Component::Start();
}

void CharacterLogic::Update() {
    if (logicState != ListeningToPlayer) {
        switch (characterMovement->GetState()) {
            case OnPathToPlayer:
            case OnPathToEnemy:
                characterAnimations->SetNewState(Running);
                break;
            case OnPathToTarget:
                characterAnimations->SetNewState(Walking);
                break;
            case NearTargetSubPoint:
                characterAnimations->SetNewState(Idle);
                break;
            case NearEnemyPosition:
                logicState = ListeningToEnemy;
                break;
            case NearPlayerPosition:
                logicState = ListeningToPlayer;
                break;
            default:
                break;
        }
    }

    if (logicState == ListeningToPlayer) {
        if (playerSatisfaction >= upperSatisfactionLimit) {
            characterAnimations->SetNewState(Cheering);
        } else if (playerSatisfaction >= middleSatisfactionLimit) {
            characterAnimations->SetNewState(Idle);
        } else if (playerSatisfaction >= lowerSatisfactionLimit)  {
            characterAnimations->SetNewState(Booing);
        } else {
            logicState = WalkingAway;
            characterMovement->SetState(ReturningToPreviousTarget);
        }
    } else if (logicState == ListeningToEnemy) {
        if (enemySatisfaction >= upperSatisfactionLimit) {
            characterAnimations->SetNewState(Cheering);
        } else if (enemySatisfaction >= middleSatisfactionLimit) {
            characterAnimations->SetNewState(Idle);
        } else if (enemySatisfaction >= lowerSatisfactionLimit)  {
            characterAnimations->SetNewState(Booing);
        } else {
            logicState = WalkingAway;
            characterMovement->SetState(ReturningToPreviousTarget);
        }
    }

    Component::Update();
}

void CharacterLogic::AIUpdate() {
    if (logicState == AlertedByPlayer) {
        CalculateSatisfaction();

        if (playerSatisfaction > minSatisfaction) {
//            characterIndicator->Indicate();
            logicState = MovingToPlayer;
            characterMovement->SetState(SettingPathToPlayer);
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
    characterAnimations.reset();
    characterMovement.reset();
    characterIndicator.reset();
    playerPattern.reset();
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
    playerPattern = nullptr;
    Component::OnDestroy();
}

/**
 * @annotation
 * Sets animation model to load.
 * @param model - path to model
 */
void CharacterLogic::SetAnimationModelToLoad(const std::string& model) {
    modelPath = model;
}

/**
 * @annotation
 * Sets player instrument and genre being used for playing.
 * @param instrument - enum for instrument name
 * @param genre - enum for music genre
 */
void CharacterLogic::SetPlayerInstrumentAndGenre(const InstrumentName& instrument, const MusicGenre& genre) {
    playerInstrumentName = instrument;
    playerGenre = genre;
}

/**
 * @annotation
 * Sets pattern played by player.
 * @param pattern - music pattern
 */
void CharacterLogic::SetPlayerPattern(const std::shared_ptr<MusicPattern>& pattern) {
    playerPattern = pattern;

    if (std::find(favPatterns.begin(), favPatterns.end(), playerPattern) != favPatterns.end())
        playerSatisfaction += 15;
    else
        playerSatisfaction -= 5;

    playerSatisfaction = std::clamp(playerSatisfaction, 0.0f, 100.0f);
}

/**
 * @annotation
 * Sets new player session state.
 * @param isPlayerPlaying - session state
 */
void CharacterLogic::SetPlayerPlayingStatus(const bool& isPlayerPlaying) {
    if (isPlayerPlaying) {
        logicState = AlertedByPlayer;
    } else {
        if (logicState != None)
            characterMovement->SetState(ReturningToPreviousTarget);

        logicState = None;
    }
}

/**
 * @annotation
 * Returns current satisfaction based on player style.
 * @returns float - player satisfaction
 */
const float CharacterLogic::GetPlayerSatisfaction() const {
    return playerSatisfaction;
}

/**
 * @annotation
 * Sets enemy instrument and genre being used for playing.
 * @param instrument - enum for instrument name
 * @param genre - enum for music genre
 */
void CharacterLogic::SetEnemyInstrumentAndGenre(const InstrumentName &instrument, const MusicGenre &genre) {
    enemyInstrumentName = instrument;
    enemyGenre = genre;
}

/**
 * @annotation
 * Sets pattern played by enemy.
 * @param pattern - music pattern
 */
void CharacterLogic::SetEnemyPattern(const std::shared_ptr<MusicPattern> &pattern) {
    enemyPattern = pattern;

    if (std::find(favPatterns.begin(), favPatterns.end(), enemyPattern) != favPatterns.end())
        enemySatisfaction += 15;
    else
        enemySatisfaction -= 5;

    enemySatisfaction = std::clamp(enemySatisfaction, 0.0f, 100.0f);
}

/**
 * @annotation
 * Sets new enemy session state.
 * @param isEnemyPlaying - session state
 */
void CharacterLogic::SetEnemyPlayingStatus(const bool& isEnemyPlaying) {
    if (isEnemyPlaying) {
        logicState = AlertedByPlayer;
    } else {
        if (logicState != None)
            characterMovement->SetState(ReturningToPreviousTarget);

        logicState = None;
    }
}

/**
 * @annotation
 * Returns current satisfaction based on enemy style.
 * @returns float - enemy satisfaction
 */
const float CharacterLogic::GetEnemySatisfaction() const {
    return enemySatisfaction;
}

/**
 * @annotation
 * Calculates starting satisfaction.
 */
void CharacterLogic::CalculateSatisfaction() {
    playerSatisfaction = 100;

    if (std::find(favGenres.begin(), favGenres.end(), playerGenre) != favGenres.end())
        playerSatisfaction += 30;

    if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), playerInstrumentName)
        != favInstrumentsNames.end())
        playerSatisfaction += 20;

    playerSatisfaction = std::clamp(playerSatisfaction, 0.0f, 100.0f);

    enemySatisfaction = 0;

    if (std::find(favGenres.begin(), favGenres.end(), enemyGenre) != favGenres.end())
        enemySatisfaction += 30;

    if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), enemyInstrumentName)
            != favInstrumentsNames.end())
        enemySatisfaction += 20;

    enemySatisfaction = std::clamp(enemySatisfaction, 0.0f, 100.0f);
//    TODO: dunno, do something here
//    if (playerSatisfaction > enemySatisfaction)

}
