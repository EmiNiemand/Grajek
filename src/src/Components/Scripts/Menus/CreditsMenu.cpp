#include "Components/Scripts/Menus/CreditsMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/Scripts/Menus/MainMenu.h"

CreditsMenu::CreditsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

CreditsMenu::~CreditsMenu() = default;

bool CreditsMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;

    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("CreditsBackButton")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}

void CreditsMenu::OnClick() {
    auto mainMenuManager = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuManager")->GetComponent<MainMenuManager>();
    HideMenu();
    mainMenuManager->activeMenu = mainMenuManager->mainMenu;
    mainMenuManager->mainMenu->ShowMenu();
    Menu::OnClick();
}