#include "GameObjectsAndPrefabs/Prefabs/MainMenuPrefab.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/LoadGameMenu.h"

MainMenuPrefab::MainMenuPrefab(const std::string &name, uint32_t id, const std::shared_ptr<GameObject> &parent, Tags tag) :
            Prefab(name, id, parent, tag) {
    prefabName = "MainMenu";
}

MainMenuPrefab::~MainMenuPrefab() = default;

std::shared_ptr<GameObject> MainMenuPrefab::Create() {
    auto mainMenuScene = shared_from_this();
    GameObject::Instantiate("MainMenuManager", mainMenuScene)->AddComponent<MainMenuManager>();

    // Main Menu
    auto mainMenu = GameObject::Instantiate("MainMenu", mainMenuScene)->AddComponent<MainMenu>();
    auto newGameButton = mainMenu->AddButton("NewGameButton", 773, 438, "UI/MainMenu/NewGameInactive.png", "UI/MainMenu/NewGameActive.png", "", 32);
    auto continueButton = mainMenu->AddButton("LoadGameButton", 785, 345, "UI/MainMenu/ContinueInactive.png", "UI/MainMenu/ContinueActive.png", "", 32);
    auto optionsButton = mainMenu->AddButton("OptionsButton", 784, 254, "UI/MainMenu/SettingsInactive.png", "UI/MainMenu/SettingsActive.png", "", 32);
    auto creditsButton = mainMenu->AddButton("CreditsButton", 810, 160, "UI/MainMenu/CreditsInactive.png", "UI/MainMenu/CreditsActive.png", "", 32);
    auto exitButton = mainMenu->AddButton("ExitFromGameButton", 877, 69, "UI/MainMenu/ExitInactive.png", "UI/MainMenu/ExitActive.png", "", 32);
    auto pauseBackground = mainMenu->AddImage("Background", 0, 0, "UI/MainMenu/MainMenu.png");
    newGameButton->previousButton = exitButton;
    newGameButton->nextButton = continueButton;
    continueButton->previousButton = newGameButton;
    continueButton->nextButton = optionsButton;
    optionsButton->previousButton = continueButton;
    optionsButton->nextButton = creditsButton;
    creditsButton->previousButton = optionsButton;
    creditsButton->nextButton = exitButton;
    exitButton->previousButton = creditsButton;
    exitButton->nextButton = newGameButton;

    // Load Game Menu
    auto loadGameMenu = GameObject::Instantiate("LoadGameMenu", mainMenuScene)->AddComponent<LoadGameMenu>();
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            loadGameMenu->Menu::AddButton("LoadGame" + std::to_string(i * 5 + j + 1), j * 300 + 50 * (j + 1), i * 300 + 100 * (i + 1), "UI/buttonInactive.png", "UI/buttonActive.png", "Save " +std::to_string(i * 5 + j + 1), 32);
        }
    }
    GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame1")->GetComponent<Button>()->previousButton = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame10")->GetComponent<Button>();
    GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame1")->GetComponent<Button>()->nextButton = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame2")->GetComponent<Button>();
    for (int i = 2; i <= 9; i++) {
        GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame" + std::to_string(i))->GetComponent<Button>()->previousButton = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame" + std::to_string(i - 1))->GetComponent<Button>();
        GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame" + std::to_string(i))->GetComponent<Button>()->nextButton = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame" + std::to_string(i + 1))->GetComponent<Button>();
    }
    GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame10")->GetComponent<Button>()->previousButton = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame9")->GetComponent<Button>();
    GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame10")->GetComponent<Button>()->nextButton = GloomEngine::GetInstance()->FindGameObjectWithName("LoadGame1")->GetComponent<Button>();
    loadGameMenu->GetParent()->DisableSelfAndChildren();

    return mainMenuScene;
}