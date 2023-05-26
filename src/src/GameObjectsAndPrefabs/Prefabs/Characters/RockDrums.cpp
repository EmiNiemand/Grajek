//
// Created by Adrian on 07.05.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/Characters/RockDrums.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/UI/Indicator.h"

RockDrums::RockDrums(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "CharacterRockDrumsPrefab";
}

RockDrums::~RockDrums() = default;

std::shared_ptr<GameObject> RockDrums::Create() {
    auto character = shared_from_this();
    auto cubeRigidbody = character->AddComponent<Rigidbody>();
    // Setting values
    auto collider = character->GetComponent<BoxCollider>();
    collider->SetOffset({0, 1.5, 0});
    collider->SetSize({1, 2, 1});
    character->transform->SetLocalScale({0.5, 0.5, 0.5});

    auto characterIndicator = character->AddComponent<Indicator>();
    characterIndicator->SetTexturePath("UI/Wykrzyknik.png");

    character->AddComponent<CharacterMovement>();
    auto characterLogic = character->AddComponent<CharacterLogic>();

    //    enum MusicGenre { Jazz = 80, RnB = 100, SynthPop=120, Rock=140 };
    characterLogic->favGenres.push_back(Rock);

    //    enum InstrumentName { Clap, Drums, Trumpet, Launchpad, Guitar };
    characterLogic->favInstrumentsNames.push_back(Drums);

//    characterLogic->favPatterns.push_back()

    return character;
}