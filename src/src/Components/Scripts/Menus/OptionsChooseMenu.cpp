#include "Components/Scripts/Menus/OptionsChooseMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/HIDManager.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "EngineManagers/OptionsManager.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "Components/UI/Button.h"
#include "EngineManagers/SceneManager.h"

OptionsChooseMenu::OptionsChooseMenu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

OptionsChooseMenu::~OptionsChooseMenu() = default;

void OptionsChooseMenu::Update() {
    auto hid = HIDManager::GetInstance();
    if (hid->IsKeyDown(Key::KEY_A) || hid->IsKeyDown(Key::KEY_ARROW_LEFT)) {
        button1->isActive = true;
        button2->isActive = false;
        return;
    }
    if (hid->IsKeyDown(Key::KEY_D) || hid->IsKeyDown(Key::KEY_ARROW_RIGHT)) {
        button1->isActive = false;
        button2->isActive = true;
        return;
    }
    if (hid->IsKeyDown(Key::KEY_ENTER)) {
        std::shared_ptr<OptionsMenu> options;
        if (SceneManager::GetInstance()->activeScene->GetName() == "Scene")
            options = GloomEngine::GetInstance()->FindGameObjectWithName("OptionsMenu")->GetComponent<OptionsMenu>();
        else
            options = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuOptionsMenu")->GetComponent<OptionsMenu>();

        if (GloomEngine::GetInstance()->FindGameObjectWithName("Player")) {
            auto player = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
            if (player->activeMenu != std::dynamic_pointer_cast<OptionsMenu>(shared_from_this())) {
                player->inputEnabled = true;
                parent->DisableSelfAndChildren();
            }
        } else {
            parent->DisableSelfAndChildren();
            auto mainMenuManager = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenuManager")->GetComponent<MainMenuManager>();
            options->HideMenu();
            mainMenuManager->activeMenu = mainMenuManager->mainMenu;
            mainMenuManager->mainMenu->ShowMenu();
            mainMenuManager->inputEnabled = true;
        }
        if (button1->isActive) {
            OptionsManager::GetInstance()->Save();
        } else {
            options->CancelSettings();
        }
    }

    Component::Update();
}

void OptionsChooseMenu::ShowChooseMenu() {
    button1->isActive = false;
    button2->isActive = true;
    parent->EnableSelfAndChildren();
}

void OptionsChooseMenu::OnDestroy() {

    Component::OnDestroy();
}