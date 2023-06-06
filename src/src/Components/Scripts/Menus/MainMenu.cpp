#include "Components/Scripts/Menus/MainMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/SceneManager.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/Scripts/Menus/LoadGameMenu.h"
#include "Components/Scripts/Menus/MainMenuOptionsMenu.h"
#include "Components/Scripts/Menus/CreditsMenu.h"

MainMenu::MainMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

MainMenu::~MainMenu() = default;

void MainMenu::Start() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("NewGameButton")->GetComponent<Button>();
    activeButton->isActive = true;
    Component::Start();
}

bool MainMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;

    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("NewGameButton")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}

void MainMenu::OnClick() {
    if (!activeButton) return;
    auto mainMenuManager = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuManager")->GetComponent<MainMenuManager>();
    if (activeButton->GetParent()->GetName() == "NewGameButton") {
        HideMenu();
        GloomEngine::GetInstance()->timeScale = 1;
        SceneManager::GetInstance()->LoadScene("Scene");
    } else if (activeButton->GetParent()->GetName() == "LoadGameButton") {
        HideMenu();
        mainMenuManager->activeMenu = mainMenuManager->loadGameMenu;
        mainMenuManager->loadGameMenu->ShowMenu();
    } else if (activeButton->GetParent()->GetName() == "MainMenuOptionsButton") {
        HideMenu();
        mainMenuManager->activeMenu = mainMenuManager->optionsMenu;
        mainMenuManager->optionsMenu->ShowMenu();
    } else if (activeButton->GetParent()->GetName() == "CreditsButton") {
        HideMenu();
        mainMenuManager->activeMenu = mainMenuManager->creditsMenu;
        mainMenuManager->creditsMenu->ShowMenu();
    } else if (activeButton->GetParent()->GetName() == "ExitFromGameButton") {
        gameShouldExit = true;
    }
    Menu::OnClick();
}
