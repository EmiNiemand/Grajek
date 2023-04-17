#include "Components/Scripts/ShopMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"

ShopMenu::ShopMenu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

ShopMenu::~ShopMenu() {}

void ShopMenu::Start() {
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>();
    Component::Start();
}

void ShopMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>();
    activeButton->isActive = true;
}

void ShopMenu::HideMenu() {
    parent->DisableSelfAndChildren();
    activeButton->isActive = false;
}

void ShopMenu::ChangeActiveButton(glm::vec2 moveVector) {
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

void ShopMenu::OnClick() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>()->isActive) {
        // TODO buy first instrument
    }
}
