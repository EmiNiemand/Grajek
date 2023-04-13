#include "Components/Scripts/PauseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"

PauseMenu::PauseMenu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

PauseMenu::~PauseMenu() {}

void PauseMenu::Start() {
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("Button")->GetComponent<Button>();
    Component::Start();
}

void PauseMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("Button")->GetComponent<Button>();
    GloomEngine::GetInstance()->FindGameObjectWithName("Button")->GetComponent<Button>()->isActive = true;
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
