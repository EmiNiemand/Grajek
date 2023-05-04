#include "GameObjectsAndPrefabs/Prefabs/Die.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"

Die::Die(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
             Prefab(name, id, parent, tag) {
    prefabName = "Die";
}

Die::~Die() = default;

std::shared_ptr<GameObject> Die::Create() {
    auto die = shared_from_this();
    auto dieRenderer = die->AddComponent<Renderer>();
    dieRenderer->LoadModel("domek/domek.obj");
    auto dieCollider = die->AddComponent<BoxCollider>();
    dieCollider->SetOffset({0, 1, 0});

    return die;
}


