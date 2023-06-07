#include "Components/Scripts/Menus/LoadGameMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/SceneManager.h"

LoadGameMenu::LoadGameMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

LoadGameMenu::~LoadGameMenu() = default;

bool LoadGameMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;

    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("Save 1")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}

void LoadGameMenu::ChangeActiveButton(glm::vec2 moveVector) {
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

void LoadGameMenu::OnClick() {
    if (!activeButton) return;
    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "Saves";
    path /= activeButton->GetParent()->GetName();
    if (!std::filesystem::exists(path.string() + ".json")) return;

    file = activeButton->GetParent()->GetName();
    GloomEngine::GetInstance()->timeScale = 1;
    SceneManager::GetInstance()->LoadScene("Scene");
    Menu::OnClick();
}