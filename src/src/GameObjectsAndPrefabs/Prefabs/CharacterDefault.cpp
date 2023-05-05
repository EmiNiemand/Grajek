//
// Created by Adrian on 05.05.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/CharacterDefault.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"

CharacterDefault::CharacterDefault(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "CharacterDefaultPrefab";
}

CharacterDefault::~CharacterDefault() = default;

std::shared_ptr<GameObject> CharacterDefault::Create() {
    auto character = shared_from_this();
    // Adding components
    auto characterRenderer = character->AddComponent<Renderer>();
    characterRenderer->LoadModel("domek/domek.obj");
    characterRenderer->material.reflection = 0.3f;
    auto cubeRigidbody = character->AddComponent<Rigidbody>();
    // Setting values
    character->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    character->transform->SetLocalScale({0.5, 0.5, 0.5});

    auto characterLogic = character->AddComponent<CharacterLogic>();
    auto characterMovement = character->AddComponent<CharacterMovement>();

    //    enum MusicGenre { Jazz = 80, RnB = 100, SynthPop=120, Rock=140 };
    characterLogic->favGenres.push_back(Jazz);
    characterLogic->favGenres.push_back(RnB);
    characterLogic->favGenres.push_back(SynthPop);
    characterLogic->favGenres.push_back(Rock);

//    enum InstrumentName { Clap, Drums, Trumpet, Launchpad, Guitar };
    characterLogic->favInstrumentsNames.push_back(Clap);
    characterLogic->favInstrumentsNames.push_back(Drums);
    characterLogic->favInstrumentsNames.push_back(Trumpet);
    characterLogic->favInstrumentsNames.push_back(Launchpad);
    characterLogic->favInstrumentsNames.push_back(Guitar);

    return character;
}