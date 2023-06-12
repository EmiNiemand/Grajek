//
// Created by Adrian on 07.05.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/Characters/RhythmicClap.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/UI/Indicator.h"
#include "Components/Scripts/Instrument.h"
#include "EngineManagers/RandomnessManager.h"

RhythmicClap::RhythmicClap(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "CharacterRhythmicClapPrefab";
}

RhythmicClap::~RhythmicClap() = default;

std::shared_ptr<GameObject> RhythmicClap::Create() {
    auto character = shared_from_this();
    auto cubeRigidbody = character->AddComponent<Rigidbody>();
    // Setting values
    auto collider = character->GetComponent<BoxCollider>();
    collider->SetOffset({0, 1.5, 0});
    collider->SetSize({1, 2, 1});
    collider->isDynamic = true;
    character->transform->SetLocalScale({0.5, 0.5, 0.5});

    auto characterIndicator = character->AddComponent<Indicator>();
    characterIndicator->SetTexturePath("UI/Wykrzyknik.png");

    character->AddComponent<CharacterMovement>();
    auto characterLogic = character->AddComponent<CharacterLogic>();

    int i = RandomnessManager::GetInstance()->GetInt(1, 5);

    std::string model;

    if (i < 10)
        model = "JazzMan00" + std::to_string(i);
    else
        model = "JazzMan0" + std::to_string(i);

    characterLogic->SetAnimationModelToLoad("Crowd/" + model + "/" + model + ".dae");

//    enum InstrumentName { Clap, Drums, Trumpet, Launchpad, Guitar };
    characterLogic->favInstrumentsNames.push_back(Clap);

//    enum MusicGenre { Rhythmic = 60, Jazz = 70, RnB = 80, SynthPop=100, Rock=120 };
    characterLogic->favGenres.push_back(Rhythmic);

    auto instrument = Instrument::GetInstrument(Clap);

    for (const auto& pat : instrument->patterns)
        characterLogic->favPatterns.push_back(pat->id);

    return character;
}