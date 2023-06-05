#include "Components/Scripts/Menus/PauseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/SceneManager.h"
#include "EngineManagers/DialogueManager.h"

PauseMenu::PauseMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

PauseMenu::~PauseMenu() = default;

bool PauseMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;

    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("ResumeButton")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}

void PauseMenu::OnClick() {
    auto buttonName = activeButton->GetParent()->GetName();
    if (buttonName == "ResumeButton") {
        HideMenu();
        //TODO: fix this
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->activeMenu.reset();
        DialogueManager::GetInstance()->NotifyMenuIsNotActive();
    } else if (buttonName == "OptionsButton") {
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
//        HideMenu();
//        GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>()->ShowMenu();
    } else if (buttonName == "Main MenuButton") {
        SceneManager::GetInstance()->LoadScene("MainMenu");
    } else if (buttonName == "ExitButton") {
        gameShouldExit = true;
    }
}