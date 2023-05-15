#include "Components/Scripts/MainMenuManager.h"
#include "GloomEngine.h"
#include "Components/UI/Menu.h"
#include "EngineManagers/HIDManager.h"
#include "Components/Scripts/PlayerInput.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "EngineManagers/OptionsManager.h"

#ifdef DEBUG
    #include <tracy/Tracy.hpp>
#endif

MainMenuManager::MainMenuManager(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

void MainMenuManager::Start() {
    mainMenu = GloomEngine::GetInstance()->FindGameObjectWithName("MainMenu")->GetComponent<MainMenu>();
    newGameMenu = GloomEngine::GetInstance()->FindGameObjectWithName("NewGameMenu")->GetComponent<NewGameMenu>();
    loadGameMenu = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGameMenu")->GetComponent<LoadGameMenu>();
    activeMenu = mainMenu;
    Component::Start();
}

void MainMenuManager::Update() {
#ifdef DEBUG
    ZoneScopedNC("Main Menu manager", 0x800080);
#endif
    PollInput();
    Component::Update();
}

void MainMenuManager::OnMenuToggle() {
    if (!activeMenu) return;
    if (activeMenu == mainMenu) return;
    // TODO uncomment later
//    if (activeMenu == optionsMenu) {
//        OptionsManager::GetInstance()->Save();
//    }
    activeMenu->HideMenu();
    activeMenu = mainMenu;
    mainMenu->ShowMenu();
}

void MainMenuManager::OnApply() const {
    if(!activeMenu) return;
    activeMenu->OnClick();
}

void MainMenuManager::OnUIMove(glm::vec2 moveVector) const {
    if(!activeMenu) return;
    activeMenu->ChangeActiveButton(moveVector);
}

void MainMenuManager::PollInput() {
    auto hid = HIDManager::GetInstance();
    glm::vec2 readMoveVector(0);

    if(activeMenu) {
        for (auto key : PlayerInput::Menu)
            if(hid->IsKeyDown(key.first)) OnMenuToggle();

        for (auto key: PlayerInput::Move) {
            if (hid->IsKeyDown(key.first)) {
                readMoveVector.y = key.second == 0 ? 1 : key.second == 2 ? -1 : readMoveVector.y;
                readMoveVector.x = key.second == 1 ? 1 : key.second == 3 ? -1 : readMoveVector.x;
            }
        }

        for (auto key : PlayerInput::Apply)
            if(hid->IsKeyDown(key.first)) OnApply();
        if(readMoveVector != glm::vec2(0))
            OnUIMove(readMoveVector);
    }
}