#include "Components/Scripts/OptionsMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/PauseMenu.h"

OptionsMenu::OptionsMenu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

OptionsMenu::~OptionsMenu() {}

void OptionsMenu::Start() {
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>();
    Component::Start();
}

void OptionsMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>();
    activeButton->isActive = true;
}

void OptionsMenu::HideMenu() {
    parent->DisableSelfAndChildren();
    activeButton->isActive = false;
}

void OptionsMenu::ChangeActiveButton(glm::vec2 moveVector) {
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

void OptionsMenu::OnClick() {
    if(GloomEngine::GetInstance()->FindGameObjectWithName("BackToPauseMenu")->GetComponent<Button>()->isActive) {
        HideMenu();
        GloomEngine::GetInstance()->FindGameObjectWithName("Pause")->GetComponent<PauseMenu>()->ShowMenu();
    }
}