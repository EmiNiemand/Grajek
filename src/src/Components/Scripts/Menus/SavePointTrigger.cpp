#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/SavePointMenu.h"
#include "EngineManagers/SavePointManager.h"

SavePointTrigger::SavePointTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

SavePointTrigger::~SavePointTrigger() = default;

void SavePointTrigger::Start() {
    savePointMenu = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenu")->GetComponent<SavePointMenu>();
    buttonImage = GloomEngine::GetInstance()->FindGameObjectWithName("SavePointMenuButtonImage")->GetComponent<Image>();
    Component::Start();
}

void SavePointTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = true;
    buttonImage->enabled = true;
    savePointMenu->triggerActive = true;
    SavePointManager::GetInstance()->activeSavePoint = std::dynamic_pointer_cast<SavePointTrigger>(shared_from_this());
    Component::OnTriggerStay(gameObject);
}

void SavePointTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    triggerActive = false;
    buttonImage->enabled = false;
    savePointMenu->triggerActive = false;
    Component::OnTriggerExit(gameObject);
}

void SavePointTrigger::OnCreate() {
    SavePointManager::GetInstance()->savePoints.insert({id, std::dynamic_pointer_cast<SavePointTrigger>(shared_from_this())});
    Component::OnCreate();
}

void SavePointTrigger::OnDestroy() {
    SavePointManager::GetInstance()->savePoints.erase(id);
    savePointMenu.reset();
    buttonImage.reset();
    Component::OnDestroy();
}