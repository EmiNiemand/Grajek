//
// Created by Adrian on 05.05.2023.
//

#include "GameObjectsAndPrefabs/Prefabs/Characters/Default.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"
#include "Components/AI/CharacterPathfinding.h"
#include "Components/UI/Indicator.h"
#include "EngineManagers/RandomnessManager.h"

Default::Default(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
        Prefab(name, id, parent, tag) {
    prefabName = "CharacterDefaultPrefab";
}

Default::~Default() = default;

std::shared_ptr<GameObject> Default::Create() {
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

    int i = RandomnessManager::GetInstance()->GetInt(1, 6);

    std::string model;

    if (i < 10)
        model = "BasicMan00" + std::to_string(i);
    else
        model = "BasicMan0" + std::to_string(i);

    characterLogic->SetAnimationModelToLoad("Crowd/" + model + "/" + model + ".dae");

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