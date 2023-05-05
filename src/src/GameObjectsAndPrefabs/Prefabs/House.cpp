#include "GameObjectsAndPrefabs/Prefabs/House.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

House::House(const std::string &name, uint32_t id, const std::shared_ptr<GameObject> &parent, Tags tag) :
            Prefab(name, id, parent, tag) {
    prefabName = "House";
}

House::~House() = default;

std::shared_ptr<GameObject> House::Create() {
    auto house = shared_from_this();
    house->AddComponent<Renderer>()->LoadModel("texturedModels/domek.obj");
    house->AddComponent<BoxCollider>()->SetOffset({0, 2, 0});
    house->GetComponent<BoxCollider>()->SetSize({3.25, 2, 1.75});

    return house;
}


