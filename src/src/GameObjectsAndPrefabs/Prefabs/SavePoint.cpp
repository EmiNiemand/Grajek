#include "GameObjectsAndPrefabs/Prefabs/SavePoint.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/SavePointTrigger.h"

SavePoint::SavePoint(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) : Prefab(
        name, id, parent, tag) {
    prefabName = "SavePoint";
}

SavePoint::~SavePoint() = default;

std::shared_ptr<GameObject> SavePoint::Create() {
    auto savePoint = shared_from_this();
    savePoint->AddComponent<Renderer>()->LoadModel("texturedModels/hydrant.obj");
    savePoint->AddComponent<BoxCollider>()->SetOffset({3.25, 0.5, 0});
    savePoint->GetComponent<BoxCollider>()->SetSize({0.5, 1, 0.5});

    auto savePointTrigger = GameObject::Instantiate("SavePointTrigger", savePoint);
    savePointTrigger->AddComponent<BoxCollider>()->SetOffset({3.25, 0.5, 0});
    savePointTrigger->GetComponent<BoxCollider>()->SetSize({1, 2, 1});
    savePointTrigger->GetComponent<BoxCollider>()->isTrigger = true;
    savePointTrigger->AddComponent<SavePointTrigger>();
//    GameObject::Instantiate("ShopButton", savePointTrigger)->AddComponent<Image>()->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png");
//    savePointTrigger->children.begin()->second->DisableSelfAndChildren();

    return savePoint;
}
