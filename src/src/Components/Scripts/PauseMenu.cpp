#include "Components/Scripts/PauseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/OptionsMenu.h"

PauseMenu::PauseMenu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

PauseMenu::~PauseMenu() {}

void PauseMenu::Start() {
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("OptionsButton")->GetComponent<Button>();
    Component::Start();
}

void PauseMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("OptionsButton")->GetComponent<Button>();
    activeButton->isActive = true;
}

void PauseMenu::HideMenu() {
    parent->DisableSelfAndChildren();
    activeButton->isActive = false;
}

void PauseMenu::ChangeActiveButton(glm::vec2 moveVector) {
    if (moveVector.y == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
    if (moveVector.y == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
}

void PauseMenu::OnClick() {
    if(GloomEngine::GetInstance()->FindGameObjectWithName("OptionsButton")->GetComponent<Button>()->isActive) {
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Options")->GetComponent<OptionsMenu>()->ShowMenu();
    } else if (GloomEngine::GetInstance()->FindGameObjectWithName("ExitToMainMenu")->GetComponent<Button>()->isActive) {
        // TODO load main menu scene
    }
}