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
#include "Components/Scripts/Instrument.h"
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
    collider->SetOffset({0, 1.5, 0.1});
    collider->SetSize({0.5, 2, 0.8});
    collider->isDynamic = true;
    character->transform->SetLocalScale({0.5, 0.5, 0.5});

    auto characterIndicator = character->AddComponent<Indicator>();
    characterIndicator->SetTexturePath("UI/Wykrzyknik.png");

    character->AddComponent<CharacterMovement>();
    auto characterLogic = character->AddComponent<CharacterLogic>();

    int modelIndex = RandomnessManager::GetInstance()->GetInt(1, 6);

    std::string model;

    if (modelIndex < 10)
        model = "BasicMan00" + std::to_string(modelIndex);
    else
        model = "BasicMan0" + std::to_string(modelIndex);

    characterLogic->SetAnimationModelToLoad("Crowd/" + model + "/" + model + ".dae");

    int randomIndex;
//    enum MusicGenre { Rhythmic = 60, Jazz = 70, RnB = 80, SynthPop=100, Rock=120 };
    std::vector<MusicGenre> randomGenres {Jazz, RnB, SynthPop, Rock, Rhythmic};

    for (int i = 0; i < 3; ++i) {
        randomIndex = RandomnessManager::GetInstance()->GetInt(0, (int)randomGenres.size() - 1);
        characterLogic->favGenres.push_back(randomGenres[randomIndex]);
        randomGenres.erase(randomGenres.cbegin() + randomIndex);
    }

//    enum InstrumentName { Clap, Drums, Trumpet, Launchpad, Guitar };
    std::vector<InstrumentName> randomInsNames {Clap, Drums, Trumpet, Launchpad, Guitar};

    for (int i = 0; i < 3; ++i) {
        randomIndex = RandomnessManager::GetInstance()->GetInt(0, (int)randomInsNames.size() - 1);
        characterLogic->favInstrumentsNames.push_back(randomInsNames[randomIndex]);
        randomInsNames.erase(randomInsNames.cbegin() + randomIndex);
    }

    auto instrument = Instrument::GetInstrument(Drums);

    for (const auto& pat : instrument->patterns)
        characterLogic->favPatterns.emplace_back(pat->id, 0.0f);

    instrument = Instrument::GetInstrument(Clap);

    for (const auto& pat : instrument->patterns)
        characterLogic->favPatterns.emplace_back(pat->id, 0.0f);

    instrument = Instrument::GetInstrument(Trumpet);

    for (const auto& pat : instrument->patterns)
        characterLogic->favPatterns.emplace_back(pat->id, 0.0f);

    return character;
}