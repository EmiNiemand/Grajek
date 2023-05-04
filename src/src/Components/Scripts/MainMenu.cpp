#include "Components/Scripts/MainMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"

MainMenu::MainMenu(const std::shared_ptr<GameObject> &parent, uint32_t id) : Menu(parent, id) {}

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
    if (activeButton->GetParent()->GetName() == "NewGameButton") {

    } else if (activeButton->GetParent()->GetName() == "LoadGameButton") {

    } else if (activeButton->GetParent()->GetName() == "OptionsButton") {

    } else if (activeButton->GetParent()->GetName() == "ExitButton") {
        gameShouldExit = true;
    }
    Menu::OnClick();
}
