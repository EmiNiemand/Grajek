#include "Components/Scripts/SavePointTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"

SavePointTrigger::SavePointTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

SavePointTrigger::~SavePointTrigger() = default;

void SavePointTrigger::OnTriggerStay(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    active = true;
    parent->children.begin()->second->EnableSelfAndChildren();
    Component::OnTriggerStay(gameObject);
}

void SavePointTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    active = false;
    parent->children.begin()->second->DisableSelfAndChildren();
    Component::OnTriggerExit(gameObject);
}