//
// Created by Adrian on 07.05.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/Characters/JazzClap.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/UI/Indicator.h"

JazzClap::JazzClap(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "CharacterJazzClapPrefab";
}

JazzClap::~JazzClap() = default;

std::shared_ptr<GameObject> JazzClap::Create() {
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
    characterLogic->favGenres.push_back(Jazz);

    //    enum InstrumentName { Clap, Drums, Trumpet, Launchpad, Guitar };
    characterLogic->favInstrumentsNames.push_back(Clap);

    return character;
}