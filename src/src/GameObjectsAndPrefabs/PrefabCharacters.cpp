//
// Created by Adrian on 03.05.2023.
//

#include <utility>
#include "GameObjectsAndPrefabs/Prefab.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/SceneManager.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/Animator.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/PlayerUI.h"
#include "Components/Scripts/Instrument.h"
#include "Components/Audio/AudioSource.h"
#include "Components/UI/Image.h"
#include "Components/AI/CharacterLogic.h"
#include "Components/AI/CharacterMovement.h"

std::vector<std::shared_ptr<GameObject> (*)(std::string)>& Prefab::GetCharacters() {
    static std::vector<std::shared_ptr<GameObject> (*)(std::string)> charactersPrefabs;

    charactersPrefabs.push_back(Prefab::GetDefaultCharacter);

    return charactersPrefabs;
}

std::shared_ptr<GameObject> Prefab::GetDefaultCharacter(std::string name) {
    auto character = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene);

    // Adding components
    auto characterRenderer = character->AddComponent<Renderer>();

    // TODO: Loading random models?
    characterRenderer->LoadModel("domek/domek.obj");
    characterRenderer->material.reflection = 0.3f;

    auto cubeRigidbody = character->AddComponent<Rigidbody>();

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

    auto characterMovement = character->AddComponent<CharacterMovement>();
    // Setting values
    character->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    character->transform->SetLocalScale({0.5, 0.5, 0.5});

    return character;
}