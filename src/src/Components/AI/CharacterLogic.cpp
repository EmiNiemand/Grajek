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
    player = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->transform;
    characterMovement = parent->GetComponent<CharacterMovement>();

    minSatisfaction = RandomnessManager::GetInstance()->GetFloat(30, 50);

    //TODO: Get session status!
//    if (Player.GetComponent<PlayerManager>().GetSessionStatus())
//    {
//        playerIsPlaying = true;
//
//        // Niesamowicie nie podoba mi się odwołanie wstecz do
//        // menadżera ale jest 02:04 i lepiej pójść na skróty
//        CrowdManager crowdMg = FindObjectOfType<CrowdManager>();
//
//        playerInstrumentName = crowdMg.GetCurrentPlayerInstrument();
//        playerGenre = crowdMg.GetCurrentPlayerGenre();
//    }

    CalculateSatisfaction();
    Component::Start();
}

void CharacterLogic::AIUpdate() {
    //Check here if the player is playing?
    // Or keep using SetPlayerStatus
    Component::AIUpdate();
}

void CharacterLogic::OnCreate() {
    Component::OnCreate();
}

void CharacterLogic::OnDestroy() {
    player = nullptr;
    characterMovement = nullptr;
    favGenres.clear();
    favInstrumentsNames.clear();
    Component::OnDestroy();
}

void CharacterLogic::Free() {
    player = nullptr;
    characterMovement = nullptr;
    favGenres.clear();
    favInstrumentsNames.clear();
}

void CharacterLogic::SetPathToPlayer() const {
    glm::vec3 newPosition = player->GetLocalPosition();
    newPosition.x - RandomnessManager::GetInstance()->GetFloat(0.5f, 2.0f);
    newPosition.z - RandomnessManager::GetInstance()->GetFloat(0.5f, 2.0f);

    characterMovement->SetNewPathToPlayer(newPosition);
}

void CharacterLogic::ReturnToPreviousPath() const {
    characterMovement->ReturnToPreviousPath();
}

void CharacterLogic::SetPlayerInstrumentAndGenre(const InstrumentName& ins, const MusicGenre& gen) {
    playerInstrumentName = ins;
    playerGenre = gen;
}

//void CharacterLogic::SetPlayerPattern() {
//    playerPattern = pat;
//
//    if (favPatterns.Contains(playerPattern)) {
//        currSatisfaction += 15;
//
//        if (currSatisfaction > 100)
//            currSatisfaction = 100;
//
//    } else
//        currSatisfaction -= 5;
//
//        if (currSatisfaction < 0)
//            currSatisfaction = 0;
//    }
//
//    if (currSatisfaction > minSatisfaction && !onPathToPlayer) {
//        onPathToPlayer = true;
//        SetPathToPlayer();
//    } else if (currSatisfaction < minSatisfaction && onPathToPlayer) {
//        onPathToPlayer = false;
//        ReturnToPreviousPath();
//    }
//
//    SetSatisfactionIndicator();
//}

void CharacterLogic::SetPlayerStatus() {
    playerIsPlaying = !playerIsPlaying;

    CalculateSatisfaction();
}

float CharacterLogic::GetCurrentSatisfaction() const {
    return currentSatisfaction;
}

void CharacterLogic::CalculateSatisfaction() {
    currentSatisfaction = 0;

    if (playerIsPlaying)
    {
        if (std::find(favGenres.begin(), favGenres.end(), playerGenre) != favGenres.end())
            currentSatisfaction += 30;


        if (std::find(favInstrumentsNames.begin(), favInstrumentsNames.end(), playerInstrumentName) != favInstrumentsNames.end())
            currentSatisfaction += 20;


        if (currentSatisfaction > minSatisfaction) {
            onPathToPlayer = true;
            SetPathToPlayer();
        }
    } else {
        onPathToPlayer = false;
        ReturnToPreviousPath();
    }

//    SetSatisfactionIndicator();
}