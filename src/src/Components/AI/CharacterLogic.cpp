//
// Created by Adrian on 01.05.2023.
//

#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "EngineManagers/AIManager.h"
#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterLogic::CharacterLogic(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterLogic::~CharacterLogic() = default;

void CharacterLogic::Start() {
    characterMovement = parent->GetComponent<CharacterMovement>();
    minSatisfaction = RandomnessManager::GetInstance()->GetFloat(30, 50);

    Component::Start();
}

void CharacterLogic::AIUpdate() {
    Component::AIUpdate();
}

void CharacterLogic::OnCreate() {
    Component::OnCreate();
}

void CharacterLogic::OnDestroy() {
    characterMovement = nullptr;
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
    Component::OnDestroy();
}

void CharacterLogic::Free() {
    characterMovement = nullptr;
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
}

void CharacterLogic::SetPathToPlayer() {
    currentState = WalkingToPlayer;

    characterMovement->SetNewPath(currentState);
}

void CharacterLogic::ReturnToPreviousPath() {
    currentState = TraversingOnPath;

    characterMovement->SetNewPath(currentState);
}

void CharacterLogic::SetPlayerInstrumentAndGenre(const InstrumentName& ins, const MusicGenre& gen) {
    playerInstrumentName = ins;
    playerGenre = gen;
}

void CharacterLogic::SetPlayerPattern(const std::shared_ptr<MusicPattern>& pat) {
    playerPattern = pat;

    if (std::find(favPatterns.begin(), favPatterns.end(), playerPattern) != favPatterns.end()) {
        currentSatisfaction += 15;

        if (currentSatisfaction > 100)
            currentSatisfaction = 100;
    } else {
        currentSatisfaction -= 5;

        if (currentSatisfaction < 0)
            currentSatisfaction = 0;
    }

    if (currentSatisfaction >= minSatisfaction && currentState == TraversingOnPath) {
        SetPathToPlayer();
    } else if (currentSatisfaction < minSatisfaction && currentState == WalkingToPlayer) {
        ReturnToPreviousPath();
    }
}

void CharacterLogic::SetPlayerPlayingStatus(bool state) {
    if (state) {
        currentState = AlertedByPlayer;
        CalculateSatisfaction();
    } else {
        ReturnToPreviousPath();
    }
}

void CharacterLogic::CalculateSatisfaction() {
    currentSatisfaction = 100;

    if (currentState == AlertedByPlayer)
    {
        if (std::find(favGenres.begin(), favGenres.end(), playerGenre) != favGenres.end())
            currentSatisfaction += 30;

        if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), playerInstrumentName) != favInstrumentsNames.end())
            currentSatisfaction += 20;

        if (currentSatisfaction > minSatisfaction) {
            SetPathToPlayer();
        }
    } else {
        ReturnToPreviousPath();
    }
}

const float CharacterLogic::GetCurrentSatisfaction() const {
    return currentSatisfaction;
}
