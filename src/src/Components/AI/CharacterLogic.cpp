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
    characterAnimator->SetAnimation("CrowdAnimations/Idle3.dae");
    characterAnimations = std::make_shared<CharacterAnimations>(characterAnimator);
    minSatisfaction = RandomnessManager::GetInstance()->GetFloat(30, 45);
    lowerSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(25, 35);
    middleSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(40, 65);
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
    if (logicState == AlertedByPlayer || logicState == AlertedByEnemy) {
        CalculateSatisfaction();

        if (playerSatisfaction > minSatisfaction) {
//            characterIndicator->Indicate();
            logicState = MovingToPlayer;
            characterMovement->SetState(SettingPathToPlayer);
        }
//        else if (enemySatisfaction > minSatisfaction) {
////            characterIndicator->Indicate();
//            logicState = MovingToEnemy;
//            characterMovement->SetState(SettingPathToEnemy);
//        }
    } else if (logicState == WalkingAway) {
        if (playerSatisfaction > minSatisfaction) {
            logicState = MovingToPlayer;
            characterMovement->SetState(SettingPathToPlayer);
        }
    }

    if (logicState == Wandering || logicState == WalkingAway) {
        playerSatisfaction = std::clamp(playerSatisfaction - SATISFACTION_REDUCER, 0.0f, 100.0f);
        enemySatisfaction = std::clamp(enemySatisfaction - SATISFACTION_REDUCER, 0.0f, 100.0f);

        time += GloomEngine::GetInstance()->deltaTime;

        if (time > PREVIOUS_SESSION_TIMEOUT) {
            time = 0.0f;
            previousPlayerGenre = {};
            previousPlayerInstrumentName = {};
            previousPlayerPattern = nullptr;
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

    if (playerPattern != nullptr) {
        if (std::find(favPatterns.begin(), favPatterns.end(), playerPattern->id) != favPatterns.end()) {
            if (previousPlayerPattern == pattern)
                playerSatisfaction += 3.75;
            else
                playerSatisfaction += 7.5f;
        }
    } else {
        playerSatisfaction -= 5.0f;
    }

    previousPlayerPattern = pattern;

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
        if (logicState != Wandering)
            characterMovement->SetState(ReturningToPreviousTarget);

        logicState = Wandering;
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

    if (enemyPattern != nullptr) {
        if (std::find(favPatterns.begin(), favPatterns.end(), enemyPattern->id) != favPatterns.end())
            enemySatisfaction += 5.0f;
    } else {
        enemySatisfaction -= 5.0f;
    }

    enemySatisfaction = std::clamp(enemySatisfaction, 0.0f, 100.0f);
}

/**
 * @annotation
 * Sets new enemy session state.
 * @param isEnemyPlaying - session state
 */
void CharacterLogic::SetEnemyPlayingStatus(const bool& isEnemyPlaying) {
    if (isEnemyPlaying) {
        logicState = AlertedByEnemy;
    } else {
        if (logicState != Wandering)
            characterMovement->SetState(ReturningToPreviousTarget);

        logicState = Wandering;
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
    if (playerSatisfaction == 0.0f) {
        if (std::find(favGenres.begin(), favGenres.end(), playerGenre) != favGenres.end())
            playerSatisfaction += 30.0f;

        if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), playerInstrumentName)
            != favInstrumentsNames.end())
            playerSatisfaction += 20.0f;

        previousPlayerGenre = playerGenre;
        previousPlayerInstrumentName = playerInstrumentName;

        playerSatisfaction = std::clamp(playerSatisfaction, 0.0f, 100.0f);
    } else {
        if (previousPlayerGenre != playerGenre && previousPlayerInstrumentName != playerInstrumentName) {
            repeatingModifier = 0.0f;
        } else {
            if (previousPlayerGenre == playerGenre)
                repeatingModifier += 3.0f;

            if (previousPlayerInstrumentName == playerInstrumentName)
                repeatingModifier += 2.0f;
        }

        playerSatisfaction = std::clamp(playerSatisfaction - repeatingModifier, 0.0f, 100.0f);
    }

    enemySatisfaction = 0.0f;

    if (std::find(favGenres.begin(), favGenres.end(), enemyGenre) != favGenres.end())
        enemySatisfaction += 30.0f;

    if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), enemyInstrumentName)
            != favInstrumentsNames.end())
        enemySatisfaction += 20.0f;

    enemySatisfaction = std::clamp(enemySatisfaction, 0.0f, 100.0f);
//    TODO: dunno, do something here
//    if (playerSatisfaction > enemySatisfaction)

}
