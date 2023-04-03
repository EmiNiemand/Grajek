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

std::shared_ptr<GameObject> Prefab::GetPlayer(std::string name) {
    auto player = GameObject::Instantiate(std::move(name), SceneManager::GetInstance()->activeScene);
    // Adding components
    auto playerRenderer = player->AddComponent<Renderer>();
    playerRenderer->LoadModel("domek/domek.obj");
    playerRenderer->material.reflection = 0.5f;
    auto cubeRigidbody = player->AddComponent<Rigidbody>();
    //Adding scripts
    player->AddComponent<PlayerMovement>();
    player->AddComponent<PlayerEquipment>();
    auto playerCanvas = GameObject::Instantiate("PlayerUI", player);
    playerCanvas->AddComponent<PlayerUI>();
    player->AddComponent<PlayerManager>();
    // Setting values
    player->GetComponent<BoxCollider>()->SetOffset({0, 1, 0});
    player->transform->SetLocalPosition({0, 2, -10});
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
