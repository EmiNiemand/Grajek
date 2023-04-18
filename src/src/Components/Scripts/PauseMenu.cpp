#include "Components/Scripts/PauseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/OptionsMenu.h"

PauseMenu::PauseMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

PauseMenu::~PauseMenu() {}

void PauseMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("OptionsButton")->GetComponent<Button>();
    activeButton->isActive = true;
}

void PauseMenu::OnClick() {
    if(GloomEngine::GetInstance()->FindGameObjectWithName("OptionsButton")->GetComponent<Button>()->isActive) {
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>()->ShowMenu();
    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("ExitToMainMenu")->GetComponent<Button>()->isActive) {
        // TODO load main menu scene
    }
}