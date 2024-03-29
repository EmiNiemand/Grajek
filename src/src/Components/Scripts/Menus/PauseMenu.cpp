#include "Components/Scripts/Menus/PauseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Audio/AudioListener.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/SceneManager.h"
#include "EngineManagers/DialogueManager.h"
#include "EngineManagers/AudioManager.h"
#include "EngineManagers/OptionsManager.h"
#include "Components/Scripts/Menus/ControlsMenu.h"

PauseMenu::PauseMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

PauseMenu::~PauseMenu() = default;

void PauseMenu::Start() {
    Component::Start();
    controlsMenu = GloomEngine::GetInstance()->FindGameObjectWithName("ControlsMenu")->GetComponent<ControlsMenu>();
}

bool PauseMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;

    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("ResumeButton")->GetComponent<Button>();
    activeButton->isActive = true;
    AudioManager::GetInstance()->audioListener->SetGain(0);
    return true;
}

void PauseMenu::HideMenu() {
    Menu::HideMenu();
    AudioManager::GetInstance()->audioListener->SetGain(OptionsManager::GetInstance()->musicVolume);

    if (!GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->session)
        SceneManager::GetInstance()->activeScene->GetComponent<AudioSource>()->SetGain(0.2f);
}

void PauseMenu::OnClick() {
    auto buttonName = activeButton->GetParent()->GetName();
    if (buttonName == "ResumeButton") {
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->activeMenu.reset();
        DialogueManager::GetInstance()->NotifyMenuIsNotActive();
    } else if (buttonName == "OptionsButton") {
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->ToggleOptionsMenu();
    } else if (buttonName == "ControlsButton") {
        controlsMenu->ShowMenu();
    } else if (buttonName == "Main MenuButton") {
        SceneManager::GetInstance()->LoadScene("MainMenu");
    } else if (buttonName == "ExitButton") {
        gameShouldExit = true;
    }
}