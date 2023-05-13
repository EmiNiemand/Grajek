#include "Components/Scripts/MainMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/SceneManager.h"

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
    if (activeButton->GetParent()->GetName() == "NewGameButton") {
        GloomEngine::GetInstance()->timeScale = 1;
        SceneManager::GetInstance()->LoadScene("Scene");
    } else if (activeButton->GetParent()->GetName() == "LoadGameButton") {

    } else if (activeButton->GetParent()->GetName() == "OptionsButton") {

    } else if (activeButton->GetParent()->GetName() == "ExitFromGameButton") {
        gameShouldExit = true;
    }
    Menu::OnClick();
}
