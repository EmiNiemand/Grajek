#include "GameObjectsAndPrefabs/Prefabs/Player.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Renderers/Animator.h"
#include "Components/PhysicsAndColliders/Rigidbody.h"
#include "Components/Renderers/Lights/SpotLight.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "LowLevelClasses/Bone.h"

Player::Player(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
                                                                                Prefab(name, id, parent, tag) {
    prefabName = "PlayerPrefab";
}

Player::~Player() = default;

std::shared_ptr<GameObject> Player::Create() {
    auto player = shared_from_this();
    //Adding scripts
    player->AddComponent<PlayerManager>();
    // Setting values
    player->transform->SetLocalScale({0.5, 0.5, 0.5});
    std::shared_ptr<GameObject> pivot = GameObject::Instantiate("PlayerPivot", player);
    pivot->transform->SetLocalPosition({0, 1, 0});

    return player;
}


