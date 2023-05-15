#include "Components/Scripts/NewGameMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/SceneManager.h"

NewGameMenu::NewGameMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

NewGameMenu::~NewGameMenu() = default;

void NewGameMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("NewGame1")->GetComponent<Button>();
    activeButton->isActive = true;
}

void NewGameMenu::ChangeActiveButton(glm::vec2 moveVector) {
    if (moveVector.x == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
    if (moveVector.x == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
}

void NewGameMenu::OnClick() {
    if (!activeButton) return;
    file = activeButton->text;
    GloomEngine::GetInstance()->timeScale = 1;
    SceneManager::GetInstance()->LoadScene("Scene");
    Menu::OnClick();
}