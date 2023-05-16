#include "GameObjectsAndPrefabs/Prefabs/MainMenuPrefab.h"
#include "Components/Scripts/MainMenuManager.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/LoadGameMenu.h"

MainMenuPrefab::MainMenuPrefab(const std::string &name, uint32_t id, const std::shared_ptr<GameObject> &parent, Tags tag) :
            Prefab(name, id, parent, tag) {
    prefabName = "MainMenu";
}

MainMenuPrefab::~MainMenuPrefab() = default;

std::shared_ptr<GameObject> MainMenuPrefab::Create() {
    auto mainMenuScene = shared_from_this();
    GameObject::Instantiate("MainMenuManager", mainMenuScene)->AddComponent<MainMenuManager>();

    // Main Menu
    std::shared_ptr<GameObject> mainMenu = GameObject::Instantiate("MainMenu", mainMenuScene);
    mainMenu->AddComponent<MainMenu>();
    std::shared_ptr<GameObject> newGameButton = mainMenu->GetComponent<MainMenu>()->Menu::AddButton("NewGameButton", 900, 600, "UI/buttonInactive.png", "UI/buttonActive.png", "New Game", 32);
    std::shared_ptr<GameObject> loadGameButton = mainMenu->GetComponent<MainMenu>()->Menu::AddButton("LoadGameButton", 900, 500, "UI/buttonInactive.png", "UI/buttonActive.png", "Load Game", 32);
    std::shared_ptr<GameObject> optionsButton = mainMenu->GetComponent<MainMenu>()->Menu::AddButton("OptionsButton", 900, 400, "UI/buttonInactive.png", "UI/buttonActive.png", "Options", 32);
    std::shared_ptr<GameObject> creditsButton = mainMenu->GetComponent<MainMenu>()->Menu::AddButton("CreditsButton", 900, 300, "UI/buttonInactive.png", "UI/buttonActive.png", "Credits", 32);
    std::shared_ptr<GameObject> exitButton = mainMenu->GetComponent<MainMenu>()->Menu::AddButton("ExitFromGameButton", 900, 200, "UI/buttonInactive.png", "UI/buttonActive.png", "Exit", 32);
    std::shared_ptr<GameObject> pauseBackground = mainMenu->GetComponent<MainMenu>()->Menu::AddImage("Background", 0, 0, "UI/MainMenu.png");
    newGameButton->GetComponent<Button>()->previousButton = exitButton->GetComponent<Button>();
    newGameButton->GetComponent<Button>()->nextButton = loadGameButton->GetComponent<Button>();
    loadGameButton->GetComponent<Button>()->previousButton = newGameButton->GetComponent<Button>();
    loadGameButton->GetComponent<Button>()->nextButton = optionsButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->previousButton = loadGameButton->GetComponent<Button>();
    optionsButton->GetComponent<Button>()->nextButton = creditsButton->GetComponent<Button>();
    creditsButton->GetComponent<Button>()->previousButton = optionsButton->GetComponent<Button>();
    creditsButton->GetComponent<Button>()->nextButton = exitButton->GetComponent<Button>();
    exitButton->GetComponent<Button>()->previousButton = creditsButton->GetComponent<Button>();
    exitButton->GetComponent<Button>()->nextButton = newGameButton->GetComponent<Button>();

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