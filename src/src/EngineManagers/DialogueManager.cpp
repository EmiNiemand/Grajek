#include "EngineManagers/DialogueManager.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/Shopkeeper.h"
#include "EngineManagers/HIDManager.h"
#include "Components/Scripts/Menus/MapTrigger.h"

DialogueManager::DialogueManager() = default;

DialogueManager::~DialogueManager() {
    delete dialogueManager;
}

DialogueManager *DialogueManager::GetInstance() {
    if (dialogueManager == nullptr) {
        dialogueManager = new DialogueManager();
    }
    return dialogueManager;
}

void DialogueManager::NotifyMenuIsActive() {
    for (const auto & dialogue : dialogues) {
        if (!dialogue.second->triggerActive) continue;
        dialogue.second->image->enabled = false;
        dialogue.second->menuActive = true;
    }
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ESC))
        shopkeeper->menuActive = true;
    map->buttonImage->enabled = false;
}

void DialogueManager::NotifyMenuIsNotActive() {
    for (const auto & dialogue : dialogues) {
        if (!dialogue.second->triggerActive) continue;
        if (!dialogue.second->forced)
            dialogue.second->image->enabled = true;
        dialogue.second->menuActive = false;
    }
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ESC))
        shopkeeper->menuActive = false;
    if (map->triggerActive)
        map->buttonImage->enabled = true;
}