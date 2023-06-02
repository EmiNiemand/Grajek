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
    auto newGameButton = mainMenu->AddButton("NewGameButton", 900, 600, "UI/buttonInactive.png", "UI/buttonActive.png", "New Game", 32);
    auto loadGameButton = mainMenu->AddButton("LoadGameButton", 900, 500, "UI/buttonInactive.png", "UI/buttonActive.png", "Load Game", 32);
    auto optionsButton = mainMenu->AddButton("OptionsButton", 900, 400, "UI/buttonInactive.png", "UI/buttonActive.png", "Options", 32);
    auto creditsButton = mainMenu->AddButton("CreditsButton", 900, 300, "UI/buttonInactive.png", "UI/buttonActive.png", "Credits", 32);
    auto exitButton = mainMenu->AddButton("ExitFromGameButton", 900, 200, "UI/buttonInactive.png", "UI/buttonActive.png", "Exit", 32);
    auto pauseBackground = mainMenu->AddImage("Background", 0, 0, "UI/MainMenu.png");
    newGameButton->previousButton = exitButton;
    newGameButton->nextButton = loadGameButton;
    loadGameButton->previousButton = newGameButton;
    loadGameButton->nextButton = optionsButton;
    optionsButton->previousButton = loadGameButton;
    optionsButton->nextButton = creditsButton;
    creditsButton->previousButton = optionsButton;
    creditsButton->nextButton = exitButton;
    exitButton->previousButton = creditsButton;
    exitButton->nextButton = newGameButton;

    // Load Game Menu
    auto loadGameMenu = GameObject::Instantiate("LoadGameMenu", mainMenuScene)->AddComponent<LoadGameMenu>();
    {
        std::vector<std::shared_ptr<Button>> loadGameButtons;
        int ypos = 700;
        for (int i = 0; i < 2; i++) {
            int xpos = 120;
            for (int j = 0; j < 5; j++) {
                std::string currentIndex = std::to_string(i * 5 + j + 1);
                loadGameButtons.push_back(loadGameMenu->Menu::AddButton(
                        "LoadGame" + currentIndex, xpos, ypos,
                        "UI/buttonSaveInactive.png", "UI/buttonSaveActive.png",
                        "Save " + currentIndex, 46));
                xpos += 350;
            }
            ypos -= 500;
        }
        loadGameButtons[0]->previousButton = loadGameButtons[9];
        loadGameButtons[0]->nextButton = loadGameButtons[1];
        for (int i = 1; i < loadGameButtons.size() - 1; i++) {
            loadGameButtons[i]->previousButton = loadGameButtons[i - 1];
            loadGameButtons[i]->nextButton = loadGameButtons[i + 1];
        }
        loadGameButtons[loadGameButtons.size() - 1]->previousButton = loadGameButtons[loadGameButtons.size() - 2];
        loadGameButtons[loadGameButtons.size() - 1]->nextButton = loadGameButtons[0];
        loadGameMenu->GetParent()->DisableSelfAndChildren();
    }

    return mainMenuScene;
}