#include "GameObjectsAndPrefabs/Prefabs/MainMenuPrefab.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/LoadGameMenu.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Menus/CreditsMenu.h"
#include "LowLevelClasses/GameData.h"
#include <fstream>
#include "nlohmann/json.hpp"

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
    auto optionsButton = mainMenu->AddButton("MainMenuOptionsButton", 784, 254, "UI/MainMenu/SettingsInactive.png", "UI/MainMenu/SettingsActive.png", "", 32);
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
                        "Save " + currentIndex, xpos, ypos,
                        "UI/buttonSaveInactive.png", "UI/buttonSaveActive.png"));

                std::filesystem::path path = std::filesystem::current_path();
                path += "\\res\\ProjectConfig\\Saves\\Save ";
                path += currentIndex;
                path += ".json";
                std::shared_ptr<GameData> gameData = std::make_shared<GameData>();
                try {
                    std::ifstream input(path);
                    nlohmann::json json;
                    input >> json;
                    json.at("saveDate").get_to(gameData->saveDate);
                }
                catch (std::exception e) {
                    spdlog::info("Failed to read a file content at path: " + path.string());
                }
                loadGameMenu->Menu::AddText("SaveDate" + currentIndex, gameData->saveDate, xpos + 15, ypos + 130,
                                            38, glm::vec3(0.1f), GameFont::MarckScript);

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
    auto optionsMenu = GameObject::Instantiate("MainMenuOptionsMenu", mainMenuScene)->AddComponent<OptionsMenu>();
    auto windowResolutionButton = optionsMenu->AddButton("MainMenuWindowResolutionButton", 60, 895, "UI/Settings/Resolution.png", "UI/Settings/ResolutionKolorowe.png");
    auto windowFullScreenButton = optionsMenu->AddButton("MainMenuWindowFullScreenButton", 60, 660, "UI/Settings/Fullscreen.png", "UI/Settings/FullscreenKolorowe.png");
    auto musicVolumeButton = optionsMenu->AddButton("MainMenuMusicVolumeButton", 60, 425, "UI/Settings/Volume.png", "UI/Settings/VolumeKolorowe.png");
    auto shadowResolutionButton = optionsMenu->AddButton("MainMenuShadowResolutionButton", 60, 190, "UI/Settings/Shadows.png", "UI/Settings/ShadowKolorowe.png");
    auto saveButton = optionsMenu->AddButton("MainMenuSaveButton", 500, 30, "UI/Settings/Save.png", "UI/Settings/SaveKolorowe.png");
    auto cancelButton = optionsMenu->AddButton("MainMenuCancelButton", 1050, 30, "UI/Settings/Cancel.png", "UI/Settings/CancelKolorowe.png");
    optionsMenu->AddButton("MainMenuWindowResolutionValue", 833, 887, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
    optionsMenu->AddButton("MainMenuWindowFullScreenValue", 833, 648, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
    optionsMenu->AddButton("MainMenuMusicVolumeValue", 833, 415, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
    optionsMenu->AddButton("MainMenuShadowResolutionValue", 833, 177, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
    optionsMenu->AddButton("MainMenuWindowResolutionLeft", 740, 867, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png")->isActive = true;
    optionsMenu->AddButton("MainMenuWindowResolutionRight", 1735, 867, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png")->isActive = true;
    optionsMenu->AddButton("MainMenuWindowFullScreenLeft", 740, 628, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png")->isActive = true;
    optionsMenu->AddButton("MainMenuWindowFullScreenRight", 1735, 628, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png")->isActive = true;
    optionsMenu->AddButton("MainMenuMusicVolumeLeft", 740, 395, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png")->isActive = true;
    optionsMenu->AddButton("MainMenuMusicVolumeRight", 1735, 395, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png")->isActive = true;
    optionsMenu->AddButton("MainMenuShadowResolutionLeft", 740, 157, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png")->isActive = true;
    optionsMenu->AddButton("MainMenuShadowResolutionRight", 1735, 157, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png")->isActive = true;
    optionsMenu->AddImage("OptionsBackground", 0, 0, "UI/Settings/Settings.png");
    windowResolutionButton->previousButton = cancelButton;
    windowResolutionButton->nextButton = windowFullScreenButton;
    windowFullScreenButton->previousButton = windowResolutionButton;
    windowFullScreenButton->nextButton = musicVolumeButton;
    musicVolumeButton->previousButton = windowFullScreenButton;
    musicVolumeButton->nextButton = shadowResolutionButton;
    shadowResolutionButton->previousButton = musicVolumeButton;
    shadowResolutionButton->nextButton = saveButton;
    saveButton->previousButton = shadowResolutionButton;
    saveButton->nextButton = cancelButton;
    cancelButton->previousButton = saveButton;
    cancelButton->nextButton = windowResolutionButton;
    optionsMenu->GetParent()->DisableSelfAndChildren();

    // Credits Menu
    auto creditsMenu = GameObject::Instantiate("CreditsMenu", mainMenuScene)->AddComponent<CreditsMenu>();
    creditsMenu->AddImage("CreditsBackground", 0, 0, "UI/Credits.png");
    creditsMenu->GetParent()->DisableSelfAndChildren();

    return mainMenuScene;
}