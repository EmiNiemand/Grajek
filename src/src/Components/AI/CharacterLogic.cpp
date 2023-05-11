//
// Created by Adrian on 01.05.2023.
//

#include "EngineManagers/RandomnessManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/Renderers/Animator.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"
#include "Components/Renderers/Camera.h"

#ifdef DEBUG
#include <tracy/Tracy.hpp>
#endif

CharacterLogic::CharacterLogic(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) { }

CharacterLogic::~CharacterLogic() = default;

//void CharacterLogic::Update() {
////    characterAnimation->PlayAnimation()
//    Component::Update();
//}

void CharacterLogic::OnCreate() {
    characterMovement = parent->GetComponent<CharacterMovement>();
//    characterAnimation = parent->GetComponent<Animator>();
    minSatisfaction = RandomnessManager::GetInstance()->GetFloat(30, 50);
    Component::OnCreate();
}

void CharacterLogic::OnDestroy() {
    characterAnimation = nullptr;
    characterMovement = nullptr;
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
    Component::OnDestroy();
}

void CharacterLogic::Free() {
    characterAnimation = nullptr;
    characterMovement = nullptr;
    favInstrumentsNames.clear();
    favGenres.clear();
    favPatterns.clear();
}

void CharacterLogic::SetPathToPlayer() {
    currentState = RunningToPlayer;

    auto camera = Camera::activeCamera->transform->GetLocalPosition();
    float x = (parent->transform->GetLocalPosition().x - camera.x + 19.0f) * 50.5f;
    float y = (parent->transform->GetLocalPosition().z - camera.z + 34.0f) * 27.0f - 540.0f;
    y = -y + 540.0f;
    auto animator = GameObject::Instantiate("Animator", parent->parent);
    auto wykrzyknik = GameObject::Instantiate("Wykrzyknik", animator)->AddComponent<Image>();
    wykrzyknik->LoadTexture((int)x, (int)y, "UI/Wykrzyknik.png");
    animator->AddComponent<UIAnimator>()->Setup(wykrzyknik, {
            {AnimatedProperty::Position, glm::vec3(x, y + 50.0f, 0.0f)}
    }, false);

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

    if (std::find(favPatterns.begin(), favPatterns.end(), playerPattern) != favPatterns.end()) {
        currentSatisfaction += 15;

        if (currentSatisfaction > 100)
            currentSatisfaction = 100;
    } else {
        currentSatisfaction -= 5;

        if (currentSatisfaction < 0)
            currentSatisfaction = 0;
    }

    if (currentSatisfaction >= minSatisfaction && currentState == WalkingOnPath) {
        SetPathToPlayer();
    } else if (currentSatisfaction < minSatisfaction && currentState == RunningToPlayer) {
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
