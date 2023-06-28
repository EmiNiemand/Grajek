#include "Components/Scripts/Menus/ControlsMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Player/PlayerManager.h"

ControlsMenu::ControlsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

ControlsMenu::~ControlsMenu() = default;

void ControlsMenu::Start() {
    Component::Start();
}

bool ControlsMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;

    GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->inputEnabled = false;
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("ControlsBackButton")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}

void ControlsMenu::OnClick() {
    HideMenu();
    GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->inputEnabled = true;
    Menu::OnClick();
}