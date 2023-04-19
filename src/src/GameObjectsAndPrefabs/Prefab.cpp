//
// Created by masterktos on 01.04.23.
//

#include "GameObjectsAndPrefabs/Prefab.h"

#include <utility>
#include "GameObjectsAndPrefabs/GameObject.h"

#include "EngineManagers/SceneManager.h"

#include "Components/Renderers/Renderer.h"
#include "Components/Renderers/Animator.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/PlayerMovement.h"
#include "Components/Scripts/PlayerManager.h"
#include "Components/Scripts/PlayerEquipment.h"
#include "Components/Scripts/PlayerUI.h"
#include "Components/Scripts/Instrument.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"

std::shared_ptr<GameObject> Prefab::GetPlayer(std::string name) {
    auto player = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene);
    // Adding components
    auto playerRenderer = player->AddComponent<Renderer>();
    playerRenderer->LoadModel("domek/domek.obj");
    playerRenderer->material.reflection = 0.5f;
    auto cubeRigidbody = player->AddComponent<Rigidbody>();
    auto listener = player->AddComponent<AudioListener>();
    //Adding scripts
    player->AddComponent<PlayerManager>();
    // Setting values
    player->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    player->transform->SetLocalPosition({0, 5, -10});
    player->transform->SetLocalScale({0.5, 1, 0.5});
    std::shared_ptr<GameObject> pivot = GameObject::Instantiate("Cube", player);
    pivot->transform->SetLocalPosition({0, 1, -10});

    return player;
}

std::shared_ptr<GameObject> Prefab::GetCube(std::string name) {
    auto sceneProp = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene, Tags::DEFAULT);
    auto scenePropRenderer = sceneProp->AddComponent<Renderer>();
    scenePropRenderer->LoadModel("domek/domek.obj");
    auto scenePropCollider = sceneProp->AddComponent<BoxCollider>();
    scenePropCollider->SetOffset({0, 1, 0});

    return sceneProp;
}

std::shared_ptr<GameObject> Prefab::GetDancingDude(std::string name) {
    auto animatedDood = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene, Tags::DEFAULT);
    auto animatedDoodAnimator = animatedDood->AddComponent<Animator>();
    animatedDoodAnimator->LoadAnimation("hiphopnigdystop/HipHopDancing.dae");

    return animatedDood;
}

std::shared_ptr<Instrument> Prefab::GetInstrument(InstrumentName instrumentName) {
    auto instrument = std::make_shared<Instrument>();
    instrument->Setup(instrumentName);

    // Add samples
    // -----------
    switch (instrumentName) {
        case Clap:
            instrument->AddSamples({
                "res/sounds/clap/clapWeak.wav",
                "res/sounds/clap/clapStrong.wav"});
            break;
        case Drums:
            instrument->AddSamples({
                "res/sounds/drums/hat.wav",
                "res/sounds/drums/kick.wav",
                "res/sounds/drums/snare.wav"});
            break;
    }

    // Add patterns
    // ------------
    switch (instrumentName) {
        case Clap: {
            instrument->GeneratePattern({
                {0, 0}, {0, 0.5}, {1, 0.5}});
            break;
        }
        case Drums: {
            instrument->GeneratePattern({
                {0, 0}, {1, 0.5}, {2, 0.5},
                {1, 0.5}});
            break;
        }
    }

    return instrument;
}
