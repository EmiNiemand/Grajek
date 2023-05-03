#include "Components/Scripts/ShopTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"

ShopTrigger::ShopTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

ShopTrigger::~ShopTrigger() {}

void ShopTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    active = true;
    parent->children.begin()->second->EnableSelfAndChildren();
    Component::OnTriggerEnter(gameObject);
}

void ShopTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    active = false;
    parent->children.begin()->second->DisableSelfAndChildren();
    Component::OnTriggerExit(gameObject);
}