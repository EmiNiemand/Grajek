#include "GameObjectsAndPrefabs/Prefabs/Player.h"
#include "EngineManagers/SceneManager.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/PlayerManager.h"

Player::Player(const std::string &name, uint32_t id, const std::shared_ptr<GameObject> &parent, Tags tag) :
                                                                                Prefab(name, id, parent, tag) {
    prefabName = "PlayerPrefab";
}

Player::~Player() = default;

std::shared_ptr<GameObject> Player::Create() {
    auto player = shared_from_this();
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


