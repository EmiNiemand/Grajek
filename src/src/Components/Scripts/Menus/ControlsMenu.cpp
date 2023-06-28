#include "Components/Scripts/Menus/ControlsMenu.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "EngineManagers/HIDManager.h"

ControlsMenu::ControlsMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

ControlsMenu::~ControlsMenu() = default;

void ControlsMenu::Start() {
    Component::Start();
}

void ControlsMenu::Update() {
    if (HIDManager::GetInstance()->IsKeyDown(Key::KEY_ESC) || HIDManager::GetInstance()->IsKeyDown(Key::KEY_ENTER)) {
        if (sameFrame) {
            sameFrame = false;
            return;
        }
        HideMenu();
        GloomEngine::GetInstance()->timeScale = 0;
        GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->inputEnabled = true;
    }
    Component::Update();
}

bool ControlsMenu::ShowMenu() {
    if(!Menu::ShowMenu()) return false;
    sameFrame = true;
    GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>()->inputEnabled = false;
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("ControlsBackButton")->GetComponent<Button>();
    activeButton->isActive = true;
    return true;
}