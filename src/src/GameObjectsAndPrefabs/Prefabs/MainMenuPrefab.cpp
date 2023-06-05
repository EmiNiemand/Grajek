#include "GameObjectsAndPrefabs/Prefabs/MainMenuPrefab.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/LoadGameMenu.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "Components/Scripts/Menus/MainMenuOptionsMenu.h"

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
    mainMenu->AddImage("MainMenuBackground", 0, 0, "UI/MainMenu/MainMenu.png");
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

    // Options Menu
    auto optionsMenu = GameObject::Instantiate("OptionsMenu", mainMenuScene)->AddComponent<MainMenuOptionsMenu>();
    auto musicVolumeButton = optionsMenu->AddButton("MusicVolumeButton", 500, 800, "UI/buttonInactive.png", "UI/buttonActive.png", "Music Volume");
    auto windowResolutionButton = optionsMenu->AddButton("WindowResolutionButton", 500, 650, "UI/buttonInactive.png", "UI/buttonActive.png", "Window Resolution");
    auto windowFullScreenButton = optionsMenu->AddButton("WindowFullScreenButton", 500, 500, "UI/buttonInactive.png", "UI/buttonActive.png", "FullScreen");
    auto shadowResolutionButton = optionsMenu->AddButton("ShadowResolutionButton", 500, 350, "UI/buttonInactive.png", "UI/buttonActive.png", "Shadow Resolution");
    auto saveButton = optionsMenu->AddButton("SaveButton", 860, 100, "UI/buttonInactive.png", "UI/buttonActive.png", "Save", 32);
    optionsMenu->AddButton("MusicVolumeValue", 1000, 800, "UI/Tekst.png", "UI/Tekst.png", "", 32);
    optionsMenu->AddButton("WindowResolutionValue", 1000, 650, "UI/Tekst.png", "UI/Tekst.png", "", 32);
    optionsMenu->AddButton("WindowFullScreenValue", 1000, 500, "UI/Tekst.png", "UI/Tekst.png", "", 32);
    optionsMenu->AddButton("ShadowResolutionValue", 1000, 350, "UI/Tekst.png", "UI/Tekst.png", "", 32);
    optionsMenu->AddButton("MusicVolumeLeft", 875, 770, "UI/GuzikLewyInactive.png", "UI/GuzikLewyActive.png")->isActive = true;
    optionsMenu->AddButton("MusicVolumeRight", 1450, 770, "UI/GuzikPrawyInactive.png", "UI/GuzikPrawyActive.png")->isActive = true;
    optionsMenu->AddButton("WindowResolutionLeft", 875, 620, "UI/GuzikLewyInactive.png", "UI/GuzikLewyActive.png")->isActive = true;
    optionsMenu->AddButton("WindowResolutionRight", 1450, 620, "UI/GuzikPrawyInactive.png", "UI/GuzikPrawyActive.png")->isActive = true;
    optionsMenu->AddButton("WindowFullScreenLeft", 875, 470, "UI/GuzikLewyInactive.png", "UI/GuzikLewyActive.png")->isActive = true;
    optionsMenu->AddButton("WindowFullScreenRight", 1450, 470, "UI/GuzikPrawyInactive.png", "UI/GuzikPrawyActive.png")->isActive = true;
    optionsMenu->AddButton("ShadowResolutionLeft", 875, 320, "UI/GuzikLewyInactive.png", "UI/GuzikLewyActive.png")->isActive = true;
    optionsMenu->AddButton("ShadowResolutionRight", 1450, 320, "UI/GuzikPrawyInactive.png", "UI/GuzikPrawyActive.png")->isActive = true;
    optionsMenu->AddImage("OptionsBackground", 0, 0, "UI/OptionsBackground.png");
    musicVolumeButton->previousButton = saveButton;
    musicVolumeButton->nextButton = windowResolutionButton;
    windowResolutionButton->previousButton = musicVolumeButton;
    windowResolutionButton->nextButton = windowFullScreenButton;
    windowFullScreenButton->previousButton = windowResolutionButton;
    windowFullScreenButton->nextButton = shadowResolutionButton;
    shadowResolutionButton->previousButton = windowFullScreenButton;
    shadowResolutionButton->nextButton = saveButton;
    saveButton->previousButton = shadowResolutionButton;
    saveButton->nextButton = musicVolumeButton;
    optionsMenu->GetParent()->DisableSelfAndChildren();

    return mainMenuScene;
}