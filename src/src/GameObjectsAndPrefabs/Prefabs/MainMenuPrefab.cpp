#include "GameObjectsAndPrefabs/Prefabs/MainMenuPrefab.h"
#include "Components/Scripts/Menus/MainMenuManager.h"
#include "Components/UI/Button.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/LoadGameMenu.h"
#include "Components/Scripts/Menus/MainMenu.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Menus/CreditsMenu.h"
#include "LowLevelClasses/GameData.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "Components/Scripts/Menus/OptionsChooseMenu.h"

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
    newGameButton->up = exitButton;
    newGameButton->down = continueButton;
    continueButton->up = newGameButton;
    continueButton->down = optionsButton;
    optionsButton->up = continueButton;
    optionsButton->down = creditsButton;
    creditsButton->up = optionsButton;
    creditsButton->down = exitButton;
    exitButton->up = creditsButton;
    exitButton->down = newGameButton;

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
                catch (std::exception e) {}
                loadGameMenu->Menu::AddText("SaveDate" + currentIndex, gameData->saveDate, xpos + 15, ypos + 130,
                                            38, glm::vec3(0.1f), GameFont::MarckScript);

                xpos += 350;
            }
            ypos -= 500;
        }

        loadGameButtons[0]->left = loadGameButtons[4];
        loadGameButtons[0]->right = loadGameButtons[1];
        loadGameButtons[0]->down = loadGameButtons[5];

        loadGameButtons[4]->left = loadGameButtons[3];
        loadGameButtons[4]->right = loadGameButtons[0];
        loadGameButtons[4]->down = loadGameButtons[9];

        loadGameButtons[5]->left = loadGameButtons[9];
        loadGameButtons[5]->right = loadGameButtons[6];
        loadGameButtons[5]->up = loadGameButtons[0];

        loadGameButtons[9]->left = loadGameButtons[8];
        loadGameButtons[9]->right = loadGameButtons[5];
        loadGameButtons[9]->up = loadGameButtons[4];

        for (int i = 1; i < 4; i++) {
            loadGameButtons[i]->left = loadGameButtons[i - 1];
            loadGameButtons[i]->right = loadGameButtons[i + 1];
            loadGameButtons[i]->down = loadGameButtons[i + 5];

            loadGameButtons[i + 5]->left = loadGameButtons[i + 4];
            loadGameButtons[i + 5]->right = loadGameButtons[i + 6];
            loadGameButtons[i + 5]->up = loadGameButtons[i];
        }

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
    windowResolutionButton->up = cancelButton;
    windowResolutionButton->down = windowFullScreenButton;
    windowFullScreenButton->up = windowResolutionButton;
    windowFullScreenButton->down = musicVolumeButton;
    musicVolumeButton->up = windowFullScreenButton;
    musicVolumeButton->down = shadowResolutionButton;
    shadowResolutionButton->up = musicVolumeButton;
    shadowResolutionButton->down = saveButton;
    saveButton->up = shadowResolutionButton;
    saveButton->down = windowResolutionButton;
    saveButton->right = cancelButton;
    cancelButton->up = shadowResolutionButton;
    cancelButton->down = windowResolutionButton;
    cancelButton->left = saveButton;

    auto chooseMenu = GameObject::Instantiate("OptionsChooseMenu", optionsMenu->GetParent())->AddComponent<OptionsChooseMenu>();
    chooseMenu->GetParent()->AddComponent<Image>()->LoadTexture(700, 420, "UI/Settings/chooseMenu.png", -0.95);
    chooseMenu->button1 = GameObject::Instantiate("OptionsChooseMenuButton1", chooseMenu->GetParent())->AddComponent<Button>();
    chooseMenu->button1->LoadTexture(772, 450, "UI/Opponent/acceptBattle.png", "UI/Opponent/acceptBattleActive.png", -1);
    chooseMenu->button2 = GameObject::Instantiate("OptionsChooseMenuButton2", chooseMenu->GetParent())->AddComponent<Button>();
    chooseMenu->button2->LoadTexture(1016, 450, "UI/Opponent/declineBattle.png", "UI/Opponent/declineBattleActive.png", -1);
    chooseMenu->button2->isActive = true;
    optionsMenu->chooseMenu = chooseMenu->GetParent();
    optionsMenu->GetParent()->DisableSelfAndChildren();

    // Credits Menu
    auto creditsMenu = GameObject::Instantiate("CreditsMenu", mainMenuScene)->AddComponent<CreditsMenu>();
    creditsMenu->AddButton("CreditsBackButton", 1300, 25, "UI/Back.png", "UI/BackKolorki.png");
    creditsMenu->AddImage("CreditsBackground", 0, 0, "UI/Credits.png");
    creditsMenu->GetParent()->DisableSelfAndChildren();

    return mainMenuScene;
}