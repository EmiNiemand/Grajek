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
#include "GameObjectsAndPrefabs/Prefab.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterLogic::CharacterLogic(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterLogic::~CharacterLogic() = default;

void CharacterLogic::Start() {
    auto animatorObject = GameObject::Instantiate("Animator", parent);
    animatorObject->transform->SetLocalRotation({0, 180, 0});
    auto characterAnimator = animatorObject->AddComponent<Animator>();
    characterAnimator->LoadAnimationModel(modelPath);
    characterAnimator->SetAnimation("CrowdAnimations/Idle3.dae");
    characterAnimations = std::make_shared<CharacterAnimations>(characterAnimator);
    lowerSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(25, 40);
    middleSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(40, 65);
    upperSatisfactionLimit = RandomnessManager::GetInstance()->GetFloat(75, 85);
    characterMovement = parent->GetComponent<CharacterMovement>();
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
            case NearTargetPosition:
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

    if (logicState == ListeningToPlayer || logicState == ListeningToEnemy) {
        if (playerSatisfaction >= upperSatisfactionLimit || enemySatisfaction >= upperSatisfactionLimit) {
            characterAnimations->SetNewState(Cheering);
        } else if (playerSatisfaction >= middleSatisfactionLimit || enemySatisfaction >= middleSatisfactionLimit) {
            characterAnimations->SetNewState(Idle);
        } else if (playerSatisfaction >= lowerSatisfactionLimit || enemySatisfaction >= lowerSatisfactionLimit) {
            characterAnimations->SetNewState(Booing);
        } else {
            logicState = WalkingAway;
            characterMovement->SetState(ReturningToPreviousTarget);
        }
    }

    Component::Update();
}

void CharacterLogic::AIUpdate() {
    if (logicState == AlertedByPlayerTalking) {
        playerSatisfaction = 50.0f;
        logicState = MovingToPlayer;
        characterMovement->SetState(SettingPathToPlayer);
    }

    if (logicState == AlertedByPlayer || logicState == AlertedByEnemy) {
        CalculateSatisfaction();

        if (playerSatisfaction > lowerSatisfactionLimit) {
            logicState = MovingToPlayer;
            characterMovement->SetState(SettingPathToPlayer);
        } else if (enemySatisfaction > lowerSatisfactionLimit) {
            logicState = MovingToEnemy;
            characterMovement->SetState(SettingPathToEnemy);
        }
    } else if (logicState == WalkingAway) {
        if (playerSatisfaction > middleSatisfactionLimit) {
            logicState = MovingToPlayer;
            characterMovement->SetState(SettingPathToPlayer);
        }
    }

    if (logicState == Wandering || logicState == WalkingAway) {
        playerSatisfaction = std::clamp(playerSatisfaction - SATISFACTION_REDUCER, 0.0f, 100.0f);
        enemySatisfaction = std::clamp(enemySatisfaction - SATISFACTION_REDUCER, 0.0f, 100.0f);

        timeSinceSession += GloomEngine::GetInstance()->AIDeltaTime;

        if (timeSinceSession > PREVIOUS_SESSION_TIMEOUT) {
            timeSinceSession = 0.0f;
            previousPlayerGenre = {};
            previousPlayerInstrumentName = {};
        }
    } else {
        timeSinceSession = 0.0f;
    }

    if (!parent->isOnFrustum) {
        timeSinceOnFrustum += GloomEngine::GetInstance()->AIDeltaTime;

        if (timeSinceOnFrustum > AI_DESPAWN_TIMEOUT) {
            timeSinceOnFrustum = 0.0f;
            GameObject::Destroy(parent);
            AIManager::GetInstance()->SpawnCharacter();
        }
    } else {
        timeSinceOnFrustum = 0.0f;
    }

    Component::AIUpdate();
}

void CharacterLogic::OnCreate() {
    AIManager::GetInstance()->charactersLogics.insert({id, std::dynamic_pointer_cast<CharacterLogic>(shared_from_this())});
    playerTransform = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
    Component::OnCreate();
}

void CharacterLogic::OnDestroy() {
    AIManager::GetInstance()->RemoveCharacterLogic(id);
    characterAnimations.reset();
    characterMovement.reset();
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
    playerTransform = nullptr;
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
    if (pattern != nullptr) {
        bool isFavorite = false;

        for (auto& pat : favPatterns) {
            if (pat.first == pattern->id) {
                playerSatisfaction += 3.0f - pat.second;
                pat.second = std::clamp(pat.second + 1.0f, 0.0f, 5.0f);
                isFavorite = true;
            } else {
                pat.second = std::clamp(pat.second - 0.75f, -1.0f, 5.0f);
            }
        }

        if (!isFavorite)
            playerSatisfaction += 1.0f;
    } else {
        playerSatisfaction -= 5.0f;
    }

    playerSatisfaction = std::clamp(playerSatisfaction, 0.0f, 100.0f);
}

/**
 * @annotation
 * Sets new player session state.
 * @param isPlayerPlaying - session state
 */
void CharacterLogic::SetPlayerPlayingStatus(const bool& isPlayerPlaying) {
    if (isPlayerPlaying) {
        playerPosition = playerTransform->GetLocalPosition();

        if (AI_AWARE_DISTANCE > glm::distance(playerPosition, parent->transform->GetGlobalPosition())) {
            logicState = AlertedByPlayer;

            for (auto &pat: favPatterns)
                pat.second = 0.0f;
        }
    } else {
        if (logicState != Wandering && characterMovement != nullptr)
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
    if (playerSatisfaction < lowerSatisfactionLimit)
        return 0.0f;

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
    if (pattern != nullptr) {
        bool isFavorite = false;

        for (auto& pat : favPatterns) {
            if (pat.first == pattern->id)
                isFavorite = true;
        }

        if (isFavorite)
            enemySatisfaction += 2.0f;
        else
            enemySatisfaction += 1.0f;
    } else {
        enemySatisfaction -= 4.0f;
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
        if (logicState != Wandering && characterMovement != nullptr)
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
                repeatingModifier += 6.0f;

            if (previousPlayerInstrumentName == playerInstrumentName)
                repeatingModifier += 4.0f;
        }

        playerSatisfaction = std::clamp(playerSatisfaction - repeatingModifier, 0.0f, 100.0f);
    }

    enemySatisfaction = -1.0f;

    if (std::find(favGenres.begin(), favGenres.end(), enemyGenre) != favGenres.end())
        enemySatisfaction += 30.0f;

    if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), enemyInstrumentName)
            != favInstrumentsNames.end())
        enemySatisfaction += 20.0f;

    enemySatisfaction = std::clamp(enemySatisfaction, 0.0f, 100.0f);
}

/**
 * @annotation
 * Returns current logic state.
 * @returns AI_LOGIC_STATE - logic state
 */
const AI_LOGIC_STATE CharacterLogic::GetLogicState() const {
    return logicState;
}

void CharacterLogic::SetAwareStatusOfOpponent(const bool& state) {
    if (state) {
        playerPosition = playerTransform->GetLocalPosition();

        if (AI_AWARE_DISTANCE > glm::distance(playerPosition, parent->transform->GetLocalPosition())) {
            logicState = AlertedByPlayerTalking;

            for (auto &pat: favPatterns)
                pat.second = 0.0f;
        }
    } else {
        if (logicState != Wandering && characterMovement != nullptr)
            characterMovement->SetState(ReturningToPreviousTarget);

        logicState = Wandering;
    }
}
