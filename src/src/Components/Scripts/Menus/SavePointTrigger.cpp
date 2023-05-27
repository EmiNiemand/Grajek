#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"

SavePointTrigger::SavePointTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

SavePointTrigger::~SavePointTrigger() = default;

void SavePointTrigger::Start() {
    buttonImage = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenuButtonImage")->GetComponent<Image>();
    Component::Start();
}

void SavePointTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    active = true;
    buttonImage->enabled = true;
    Component::OnTriggerStay(gameObject);
}

void SavePointTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    active = false;
    buttonImage->enabled = false;
    Component::OnTriggerExit(gameObject);
}