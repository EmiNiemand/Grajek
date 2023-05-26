#include "GameObjectsAndPrefabs/Prefabs/OpponentPrefab.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

OpponentPrefab::OpponentPrefab(const std::string &name, int id, const std::shared_ptr<GameObject>& parent, Tags tag)
        : Prefab(name, id, parent, tag) {
    prefabName = "Opponent";
}

OpponentPrefab::~OpponentPrefab() = default;

std::shared_ptr<GameObject> OpponentPrefab::Create() {
    auto opponent = shared_from_this();
    opponent->AddComponent<Renderer>()->LoadModel("texturedModels/przeciwnik.obj");
    opponent->AddComponent<BoxCollider>()->SetSize({2, 1, 2});

    auto trigger = GameObject::Instantiate("OpponentTrigger", opponent);
    trigger->AddComponent<BoxCollider>()->SetSize({6, 1, 6});
    trigger->GetComponent<BoxCollider>()->isTrigger = true;

    return opponent;
}