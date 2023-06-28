#include "EngineManagers/SavePointManager.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "Components/UI/Image.h"

SavePointManager::SavePointManager() = default;

SavePointManager::~SavePointManager() {
    activeSavePoint.reset();
    delete savePointManager;
}

SavePointManager *SavePointManager::GetInstance() {
    if (savePointManager == nullptr) {
        savePointManager = new SavePointManager();
    }
    return savePointManager;
}

void SavePointManager::NotifyMenuIsActive() const {
    if (!activeSavePoint) return;
    activeSavePoint->buttonImage->enabled = false;
}

void SavePointManager::NotifyMenuIsNotActive() const {
    if (!activeSavePoint) return;
    activeSavePoint->buttonImage->enabled = true;
}