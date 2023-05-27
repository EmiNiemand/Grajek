#include "EngineManagers/SavePointManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "Components/UI/Image.h"

SavePointManager::SavePointManager() = default;

SavePointManager::~SavePointManager() {
    delete savePointManager;
}

SavePointManager *SavePointManager::GetInstance() {
    if (savePointManager == nullptr) {
        savePointManager = new SavePointManager();
    }
    return savePointManager;
}

void SavePointManager::NotifyMenuIsActive() {
    if (!activeSavePoint) return;
    if (!activeSavePoint->GetParent()->children.empty())
        GameObject::Destroy(activeSavePoint->GetParent()->children.begin()->second);
    for (const auto & savePoint : savePoints) {
        if (savePoint->triggerActive) {
            buttonImage->enabled = false;
            return;
        }
    }
}

void SavePointManager::NotifyMenuIsNotActive() {
    for (const auto & savePoint : savePoints) {
        if (savePoint->triggerActive) {
            buttonImage->enabled = true;
            return;
        }
    }
}