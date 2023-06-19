//
// Created by Adrian on 07.05.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/Characters/JazzTrumpet.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/Scripts/Instrument.h"
#include "EngineManagers/RandomnessManager.h"

JazzTrumpet::JazzTrumpet(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "CharacterJazzTrumpetPrefab";
}

JazzTrumpet::~JazzTrumpet() = default;

std::shared_ptr<GameObject> JazzTrumpet::Create() {
    auto character = shared_from_this();
    auto cubeRigidbody = character->AddComponent<Rigidbody>();
    // Setting values
    auto collider = character->GetComponent<BoxCollider>();
    collider->SetOffset({0, 1.5, 0.1});
    collider->SetSize({0.5, 2, 0.6});
    collider->isDynamic = true;
    character->transform->SetLocalScale({0.5, 0.5, 0.5});

    character->AddComponent<CharacterMovement>();
    auto characterLogic = character->AddComponent<CharacterLogic>();

    int modelIndex = RandomnessManager::GetInstance()->GetInt(1, 5);

    std::string model;

    if (modelIndex < 10)
        model = "JazzMan00" + std::to_string(modelIndex);
    else
        model = "JazzMan0" + std::to_string(modelIndex);

    characterLogic->SetAnimationModelToLoad("Crowd/" + model + "/" + model + ".dae");

//    enum MusicGenre { Rhythmic = 60, Jazz = 70, RnB = 80, SynthPop=100, Rock=120 };
    characterLogic->favGenres.push_back(Jazz);

//    enum InstrumentName { Clap, Drums, Trumpet, Launchpad, Guitar };
    characterLogic->favInstrumentsNames.push_back(Trumpet);

    auto instrument = Instrument::GetInstrument(Trumpet);

    for (const auto& pat : instrument->patterns)
        characterLogic->favPatterns.emplace_back(pat->id, 0.0f);

    return character;
}
