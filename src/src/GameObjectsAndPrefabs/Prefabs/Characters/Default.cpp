//
// Created by Adrian on 05.05.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/Characters/Default.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/UI/Indicator.h"

Default::Default(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "CharacterDefaultPrefab";
}

Default::~Default() = default;

std::shared_ptr<GameObject> Default::Create() {
    auto character = shared_from_this();
    // Adding components
    auto characterRenderer = character->AddComponent<Renderer>();
    characterRenderer->LoadModel("domek/domek.obj");
    characterRenderer->material.reflection = 0.3f;
    auto cubeRigidbody = character->AddComponent<Rigidbody>();
    // Setting values
    character->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    character->transform->SetLocalScale({0.5, 0.5, 0.5});

    auto characterIndicator = character->AddComponent<Indicator>();
    characterIndicator->SetTexturePath("UI/Wykrzyknik.png");
    auto characterMovement = character->AddComponent<CharacterMovement>();
    auto characterLogic = character->AddComponent<CharacterLogic>();

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

//    auto animatedDoodAnimator = animatedDood->AddComponent<Animator>();
//    animatedDoodAnimator->LoadAnimation("Animacje/Idle.dae");
//
//    animatedDoodAnimator2->LoadAnimationModel("Animacje/Idle.dae");
//    animatedDoodAnimator2->SetAnimation("Animacje/Idle.dae");
//    animatedDood2->transform->SetLocalPosition({6, 1, -10});
//    animatedDood2->transform->SetLocalRotation({0, -90, 0});
//    animatedDood2->transform->SetLocalScale({0.25, 0.25, 0.25});

    return character;
}