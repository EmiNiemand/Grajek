#include "EngineManagers/DialogueManager.h"
#include "Components/Scripts/Menus/Dialogue.h"
#include "Components/UI/Image.h"

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
        if (!dialogue->triggerActive) continue;
        dialogue->image->enabled = false;
        dialogue->menuActive = true;
    }
}

void DialogueManager::NotifyMenuIsNotActive() {
    for (const auto & dialogue : dialogues) {
        if (!dialogue->triggerActive) continue;
        if (!dialogue->forced)
            dialogue->image->enabled = true;
        dialogue->menuActive = false;
    }
}