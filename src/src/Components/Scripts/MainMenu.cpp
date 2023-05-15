#include "Components/Scripts/MainMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/SceneManager.h"
#include "Components/Scripts/MainMenuManager.h"

MainMenu::MainMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

MainMenu::~MainMenu() = default;

void MainMenu::Start() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("NewGameButton")->GetComponent<Button>();
    activeButton->isActive = true;
    Component::Start();
}

void MainMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("NewGameButton")->GetComponent<Button>();
    activeButton->isActive = true;
}

void MainMenu::OnClick() {
    if (!activeButton) return;
    auto mainMenuManager = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuManager")->GetComponent<MainMenuManager>();
    if (activeButton->GetParent()->GetName() == "NewGameButton") {
        HideMenu();
        mainMenuManager->activeMenu = mainMenuManager->newGameMenu;
        mainMenuManager->newGameMenu->ShowMenu();
    } else if (activeButton->GetParent()->GetName() == "LoadGameButton") {
        HideMenu();
        mainMenuManager->activeMenu = mainMenuManager->loadGameMenu;
        mainMenuManager->loadGameMenu->ShowMenu();
    } else if (activeButton->GetParent()->GetName() == "OptionsButton") {

    } else if (activeButton->GetParent()->GetName() == "ExitFromGameButton") {
        gameShouldExit = true;
    }
    Menu::OnClick();
}
