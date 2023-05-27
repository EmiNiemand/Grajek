#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/SavePointMenu.h"

SavePointTrigger::SavePointTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

SavePointTrigger::~SavePointTrigger() = default;

void SavePointTrigger::Start() {
    buttonImage = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenuButtonImage")->GetComponent<Image>();
    savePointMenu = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenu")->GetComponent<SavePointMenu>();
    Component::Start();
}

void SavePointTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    buttonImage->enabled = true;
    savePointMenu->triggerActive = true;
    Component::OnTriggerStay(gameObject);
}

void SavePointTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    buttonImage->enabled = false;
    savePointMenu->triggerActive = false;
    Component::OnTriggerExit(gameObject);
}