#include "Components/Scripts/Menus/SavePointMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "EngineManagers/DataPersistanceManager.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Scripts/Menus/SavePointTrigger.h"
#include "Components/UI/Image.h"
#include "Components/Animations/UIAnimator.h"
#include <filesystem>

SavePointMenu::SavePointMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

SavePointMenu::~SavePointMenu() = default;

void SavePointMenu::Awake() {
    // TODO add savepoints
    triggers.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("SavePointTrigger")->GetComponent<SavePointTrigger>());
    Component::Start();
}

bool SavePointMenu::ShowMenu() {
    for (const auto & trigger : triggers) {
        if (trigger->active) {
            parent->EnableSelfAndChildren();
            buttonImage->enabled = false;
            activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("Save1")->GetComponent<Button>();
            activeButton->isActive = true;
            return true;
        }
    }
    return false;
}

void SavePointMenu::ChangeActiveButton(glm::vec2 moveVector) {
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

void SavePointMenu::OnClick() {
    if (!activeButton) return;
    buttonImage->enabled = true;
    auto animator = GameObject::Instantiate("SavePointMenuAnimator", parent->parent);
    auto image = GameObject::Instantiate("SavePointMenuImage", animator)->AddComponent<Image>();
    image->LoadTexture(1000, 700, "UI/ZapisGry.png");
    animator->AddComponent<UIAnimator>()->Setup(image, {
            {AnimatedProperty::Alpha, glm::vec3(0.0f), 2.0f}
    }, AnimationBehaviour::OneTime);

    std::filesystem::path path = std::filesystem::current_path();
    path /= "res";
    path /= "ProjectConfig";
    path /= "Saves";
    DataPersistanceManager::GetInstance()->SaveGame(path.string(), activeButton->text);
    GloomEngine::GetInstance()->timeScale = 1;
    HideMenu();
    GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->activeMenu.reset();
    Menu::OnClick();
}

void SavePointMenu::HideMenu() {
    Menu::HideMenu();
    buttonImage->enabled = true;
}