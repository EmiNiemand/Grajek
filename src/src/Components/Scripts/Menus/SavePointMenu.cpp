#include "Components/Scripts/Menus/SavePointMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/DataPersistanceManager.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Scripts/Menus/SavePointTrigger.h"
#include <filesystem>

SavePointMenu::SavePointMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

SavePointMenu::~SavePointMenu() = default;

void SavePointMenu::Start() {
    triggers.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("SavePointTrigger")->GetComponent<SavePointTrigger>());
    Component::Start();
}

bool SavePointMenu::ShowMenu() {
    for (const auto & trigger : triggers) {
        if (!trigger->active) return false;
    }
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("Save1")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}

void SavePointMenu::ChangeActiveButton(glm::vec2 moveVector) {
    if (moveVector.x == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
    if (moveVector.x == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
}

void SavePointMenu::OnClick() {
    if (!activeButton) return;
    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "Saves";
    DataPersistanceManager::GetInstance()->SaveGame(path.string(), activeButton->text);
    GloomEngine::GetInstance()->timeScale = 1;
    HideMenu();
    GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->activeMenu.reset();
    Menu::OnClick();
}