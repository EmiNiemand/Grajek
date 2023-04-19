#include "Components/Scripts/PauseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/OptionsMenu.h"
#include "Components/Scripts/PlayerManager.h"

PauseMenu::PauseMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

PauseMenu::~PauseMenu() {}

void PauseMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("OptionsButton")->GetComponent<Button>();
    activeButton->isActive = true;
}

void PauseMenu::OnClick() {
    if(GloomEngine::GetInstance()->FindGameObjectWithName("OptionsButton")->GetComponent<Button>()->isActive) {
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->pauseActive = false;
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->uiActive = true;
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>()->ShowMenu();
    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("ExitToMainMenu")->GetComponent<Button>()->isActive) {
        // TODO load main menu scene
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->pauseActive = false;
        gameShouldExit = true;
    }
}