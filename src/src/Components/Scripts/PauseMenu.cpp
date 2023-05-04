#include "Components/Scripts/PauseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/OptionsMenu.h"
#include "Components/Scripts/PlayerManager.h"

PauseMenu::PauseMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

PauseMenu::~PauseMenu() = default;

void PauseMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("ResumeButton")->GetComponent<Button>();
    activeButton->isActive = true;
}

void PauseMenu::OnClick() {
    if (activeButton->GetParent()->GetName() == "ResumeButton") {
        GloomEngine::GetInstance()->timeScale = 1;
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->activeMenu.reset();
    } else if (activeButton->GetParent()->GetName() == "OptionsButton") {
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>()->ShowMenu();
    } else if (activeButton->GetParent()->GetName() == "ExitToMainMenuButton") {
        // TODO load main menu scene
    } else if (activeButton->GetParent()->GetName() == "ExitButton") {
        gameShouldExit = true;
    }
}