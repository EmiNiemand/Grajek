//
// Created by masterktos on 03.04.23.
//

#include "Components/Scripts/Player/PlayerUI.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Text.h"
#include "Utilities.h"
#include "EngineManagers/SceneManager.h"
#include "Components/Scripts/Menus/PauseMenu.h"
#include "Components/UI/Button.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/OptionsMenu.h"
#include "Components/Scripts/Menus/SavePointMenu.h"
#include "Components/Scripts/Menus/ShopMenu.h"
#include "Components/Scripts/Menus/ControlsMenu.h"
#include "EngineManagers/SavePointManager.h"
#include "Components/Animations/UIAnimator.h"
#include "LowLevelClasses/GameData.h"
#include "Components/Scripts/Menus/OptionsChooseMenu.h"
#include "Components/Scripts/Instrument.h"
#include <fstream>

PlayerUI::PlayerUI(const std::shared_ptr<GameObject> &parent, int id)
        : Component(parent, id) {
    cashText = GameObject::Instantiate("Money", parent)->AddComponent<Text>();
    cashText->LoadFont("$ 0", 10, 1010, 64, glm::vec3(1));
    cashText->z = -0.85;

    cashBackground =  GameObject::Instantiate("UI", parent)->AddComponent<Image>();
    cashBackground->LoadTexture(0, 0, "UI/MoneyBackground.png", -0.8);
    cashBackground->SetPosition(0, 1080 - cashBackground->GetHeight());
    cashAmount = 0;

    auto badgesParent = GameObject::Instantiate("Badges", parent);
    {
        auto badgesBG = badgesParent->AddComponent<Image>();
        badgesBG->LoadTexture(0, 0, "UI/Badges/Background.png", -0.82);
        badgesBG->SetPosition(0, 1080 - cashBackground->GetHeight() - badgesBG->GetHeight() + 12);

        int xPos = 18, yPos = 1080 - cashBackground->GetHeight() - 73;
        float zPos = -0.85;

        badges = {
            {PlayerBadges::DRUMS, nullptr}, {PlayerBadges::TRUMPET, nullptr},
            {PlayerBadges::LAUNCHPAD, nullptr}, {PlayerBadges::GUITAR, nullptr}
        };
        for(auto &badge : badges) {
            badge.second = GameObject::Instantiate("DrumsBadge", badgesParent)->AddComponent<Image>();
            badge.second->SetAlpha(0);
        }
        badges[PlayerBadges::DRUMS]->LoadTexture(xPos, yPos + 1, "UI/Badges/Drums.png", zPos);
        xPos += 57;
        badges[PlayerBadges::TRUMPET]->LoadTexture(xPos, yPos - 7, "UI/Badges/Trumpet.png", zPos);
        xPos += 55;
        badges[PlayerBadges::LAUNCHPAD]->LoadTexture(xPos, yPos - 8, "UI/Badges/Launchpad.png", zPos);
        xPos += 67;
        badges[PlayerBadges::GUITAR]->LoadTexture(xPos, yPos - 5, "UI/Badges/Guitar.png", zPos);
    }

    auto menus = GameObject::Instantiate("Menus", parent);

    // Set up pause menu
    auto pause = GameObject::Instantiate("Pause", menus)->AddComponent<PauseMenu>();
    {
        std::vector<std::string> buttonNames = {
                "Resume",
                "Options",
                "Controls",
                "Main Menu",
                "Exit"
        };
        std::vector<std::string> buttonImageNames = {
                "Resume",
                "Options",
                "Controls",
                "MainMenu",
                "Exit"
        };

        int buttonOffset = -140;
        int currentYPos = 650;

        std::vector<std::shared_ptr<Button>> buttonBuffer;

        for (int i = 0; i < buttonNames.size(); ++i) {
            auto optionButton = pause->AddButton(buttonNames[i]+"Button", 0, 0,
                                                 "UI/Pauza/"+buttonImageNames[i]+".png",
                                                 "UI/Pauza/"+buttonImageNames[i]+"_Kolor.png",
                                                 "", 32, glm::vec3(), GameFont::KanitLight, -0.96);
            optionButton->ChangePosition(50, currentYPos);
            currentYPos += buttonOffset;

            buttonBuffer.push_back(optionButton);
        }

        pause->AddImage("Background", 0, 0, "UI/Pauza/PauzaBG.png", -0.95);

        for(int i=0; i<buttonNames.size(); i++) {
            auto prevIndex = i-1<0 ? buttonNames.size()-1:i-1;
            auto nextIndex = i+1==buttonNames.size() ? 0:i+1;
            buttonBuffer[i]->up = buttonBuffer[prevIndex];
            buttonBuffer[i]->down = buttonBuffer[nextIndex];
        }

        pause->GetParent()->DisableSelfAndChildren();
    }

    // Set up options menu
    auto optionsMenu = GameObject::Instantiate("OptionsMenu", menus)->AddComponent<OptionsMenu>();
    {
        auto windowResolutionButton = optionsMenu->AddButton("WindowResolutionButton", 60, 895, "UI/Settings/Resolution.png", "UI/Settings/ResolutionKolorowe.png");
        windowResolutionButton->ChangeZ(-0.95);
        auto windowFullScreenButton = optionsMenu->AddButton("WindowFullScreenButton", 60, 660, "UI/Settings/Fullscreen.png", "UI/Settings/FullscreenKolorowe.png");
        windowFullScreenButton->ChangeZ(-0.95);
        auto musicVolumeButton = optionsMenu->AddButton("MusicVolumeButton", 60, 425, "UI/Settings/Volume.png", "UI/Settings/VolumeKolorowe.png");
        musicVolumeButton->ChangeZ(-0.95);
        auto shadowResolutionButton = optionsMenu->AddButton("ShadowResolutionButton", 60, 190, "UI/Settings/Shadows.png", "UI/Settings/ShadowKolorowe.png");
        shadowResolutionButton->ChangeZ(-0.95);
        auto saveButton = optionsMenu->AddButton("SaveButton", 500, 30, "UI/Settings/Save.png", "UI/Settings/SaveKolorowe.png");
        saveButton->ChangeZ(-0.95);
        auto cancelButton = optionsMenu->AddButton("CancelButton", 1050, 30, "UI/Settings/Cancel.png", "UI/Settings/CancelKolorowe.png");
        cancelButton->ChangeZ(-0.95);
        auto button = optionsMenu->AddButton("WindowResolutionValue", 833, 887, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
        button->ChangeZ(-0.93);
        button = optionsMenu->AddButton("WindowFullScreenValue", 833, 648, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
        button->ChangeZ(-0.93);
        button = optionsMenu->AddButton("MusicVolumeValue", 833, 415, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
        button->ChangeZ(-0.93);
        button = optionsMenu->AddButton("ShadowResolutionValue", 833, 177, "UI/Settings/Pasek.png", "UI/Settings/Pasek.png", "", 32);
        button->ChangeZ(-0.93);
        button = optionsMenu->AddButton("WindowResolutionLeft", 740, 867, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png");
        button->isActive=true; button->ChangeZ(-0.95);
        button = optionsMenu->AddButton("WindowResolutionRight", 1735, 867, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png");
        button->isActive=true; button->ChangeZ(-0.95);
        button = optionsMenu->AddButton("WindowFullScreenLeft", 740, 628, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png");
        button->isActive=true; button->ChangeZ(-0.95);
        button = optionsMenu->AddButton("WindowFullScreenRight", 1735, 628, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png");
        button->isActive=true; button->ChangeZ(-0.95);
        button = optionsMenu->AddButton("MusicVolumeLeft", 740, 395, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png");
        button->isActive=true; button->ChangeZ(-0.95);
        button = optionsMenu->AddButton("MusicVolumeRight", 1735, 395, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png");
        button->isActive=true; button->ChangeZ(-0.95);
        button = optionsMenu->AddButton("ShadowResolutionLeft", 740, 157, "UI/Settings/GuzikNieKlikniety.png", "UI/Settings/GuzikKlikniety.png");
        button->isActive=true; button->ChangeZ(-0.95);
        button = optionsMenu->AddButton("ShadowResolutionRight", 1735, 157, "UI/Settings/GuzikNieKliknietyPrawy.png", "UI/Settings/GuzikKliknietyPrawy.png");
        button->isActive=true; button->ChangeZ(-0.95);
        optionsMenu->AddImage("OptionsBackground", 0, 0, "UI/Settings/Settings.png", -0.9);
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
        chooseMenu->GetParent()->AddComponent<Image>()->LoadTexture(700, 420, "UI/Settings/chooseMenu.png", -0.99);
        GameObject::Instantiate("OptionsChooseMenuBG", chooseMenu->GetParent())->AddComponent<Image>()->LoadTexture(0, 0, "UI/backgroundOpacity60.png", -0.98);
        chooseMenu->button1 = GameObject::Instantiate("OptionsChooseMenuButton1", chooseMenu->GetParent())->AddComponent<Button>();
        chooseMenu->button1->LoadTexture(772, 450, "UI/Settings/chooseMenuYes.png", "UI/Settings/chooseMenuYesActive.png", -1);
        chooseMenu->button2 = GameObject::Instantiate("OptionsChooseMenuButton2", chooseMenu->GetParent())->AddComponent<Button>();
        chooseMenu->button2->LoadTexture(1016, 450, "UI/Settings/chooseMenuNo.png", "UI/Settings/chooseMenuNoActive.png", -1);
        chooseMenu->button2->isActive = true;
        optionsMenu->chooseMenu = chooseMenu->GetParent();
        optionsMenu->GetParent()->DisableSelfAndChildren();
    }

    // Set up shop menu
    auto shopMenu = GameObject::Instantiate("ShopMenu", menus);
    {
        auto shopMenuComponent = shopMenu->AddComponent<ShopMenu>();
        auto firstInstrumentCost = shopMenuComponent->Menu::AddText("FirstInstrumentCost", "Cost: $" + std::to_string(DRUMS_PRICE), 320, 220, 32,glm::vec3(1.0f, 1.0f, 1.0f));
        auto secondInstrumentCost = shopMenuComponent->Menu::AddText("SecondInstrumentCost", "Cost: $" + std::to_string(TRUMPET_PRICE), 1600, 620, 32,glm::vec3(1.0f, 1.0f, 1.0f));
        auto thirdInstrumentCost = shopMenuComponent->Menu::AddText("ThirdInstrumentCost", "Sold out!", 1790, 350, 32,glm::vec3(1.0f, 1.0f, 1.0f));
        auto fourthInstrumentCost = shopMenuComponent->Menu::AddText("FourthInstrumentCost", "Sold out!", 1035, 900,32, glm::vec3(1.0f, 1.0f, 1.0f));
        auto firstInstrument = shopMenuComponent->Menu::AddButton("FirstInstrument", 10, 0, "UI/Sklep/Perkusja.png","UI/Sklep/PerkusjaZRamka.png");
        auto secondInstrument = shopMenuComponent->Menu::AddButton("SecondInstrument", 1425, 525, "UI/Sklep/Trabka.png","UI/Sklep/TrabkaZRamka.png");
        auto thirdInstrument = shopMenuComponent->Menu::AddButton("ThirdInstrument", 1525, 250,"UI/Sklep/LaunbhPad.png","UI/Sklep/LaunbhPadZRamka.png");
        auto fourthInstrument = shopMenuComponent->Menu::AddButton("FourthInstrument", 600, 700, "UI/Sklep/Gitara.png","UI/Sklep/GitaraZRamka.png");
        shopMenuComponent->Menu::AddImage("ExitImage", 1600, 50, "UI/Sklep/Przycisk2.png");
        shopMenuComponent->buyImage = shopMenuComponent->Menu::AddImage("BuyImage", 1300, 50, "UI/Sklep/buyInstrument.png");
        shopMenuComponent->Menu::AddImage("ShopBackground", 0, 0, "UI/Sklep/Sklep.png");
        shopMenuComponent->Menu::AddImage("ShopHead", 700, 630, "UI/Sklep/Head.png", -0.05f);
        firstInstrumentCost->GetParent()->SetParent(firstInstrument->GetParent());
        secondInstrumentCost->GetParent()->SetParent(secondInstrument->GetParent());
        thirdInstrumentCost->GetParent()->SetParent(thirdInstrument->GetParent());
        fourthInstrumentCost->GetParent()->SetParent(fourthInstrument->GetParent());
        firstInstrument->left = thirdInstrument;
        firstInstrument->right = fourthInstrument;
        secondInstrument->left = fourthInstrument;
        secondInstrument->right = thirdInstrument;
        thirdInstrument->left = secondInstrument;
        thirdInstrument->right = firstInstrument;
        fourthInstrument->left = firstInstrument;
        fourthInstrument->right = secondInstrument;
        shopMenu->DisableSelfAndChildren();
    }

    // Set up save point menu
    auto savePointMenu = GameObject::Instantiate("SavePointMenu", menus)->AddComponent<SavePointMenu>();
    {
        std::vector<std::shared_ptr<Button>> loadGameButtons;
        int ypos = 700;
        for (int i = 0; i < 2; i++) {
            int xpos = 120;
            for (int j = 0; j < 5; j++) {
                std::string currentIndex = std::to_string(i * 5 + j + 1);
                loadGameButtons.push_back(savePointMenu->Menu::AddButton(
                        "Save" + currentIndex, xpos, ypos,
                        "UI/buttonSaveInactive.png", "UI/buttonSaveActive.png",
                        "Save " + currentIndex, 0, glm::vec3(1)));
                loadGameButtons.back()->ChangeZ(-0.93);

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
                auto dateText = savePointMenu->AddText(
                        "SaveDate" + currentIndex, gameData->saveDate, xpos + 15, ypos + 130,
                        38, glm::vec3(0.1f), GameFont::MarckScript);
                dateText->z = -0.95;
                savePointMenu->saveDates.push_back(dateText);

                xpos += 350;
            }
            ypos -= 500;
        }

        savePointMenu->Menu::AddImage("SaveMenuBackground", 0, 0, "UI/backgroundOpacity60.png");

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

        savePointMenu->GetParent()->DisableSelfAndChildren();
    }

    // Set up controls menu
    auto controlsMenu = GameObject::Instantiate("ControlsMenu", menus)->AddComponent<ControlsMenu>();
    controlsMenu->Menu::AddImage("ControlsMenuBackground", 0, 0, "UI/Controls.png", -0.9);
    controlsMenu->Menu::AddButton("ControlsBackButton", 1500, 25, "UI/BackKolorki.png", "UI/BackKolorki.png", "", 18, glm::vec3(0.0f, 0.0f, 0.0f), "Kanit-Light.ttf", -0.95);
    controlsMenu->GetParent()->DisableSelfAndChildren();
}

void PlayerUI::UpdateCash(int newAmount, bool playAnimation) {
	cashText->text = "$ " + std::to_string(newAmount);

    int cashDiff = newAmount - cashAmount;
    cashAmount = newAmount;

    if (cashDiff == 0) return;
    if (!playAnimation) return;

    auto addMoneyImage = GameObject::Instantiate("AddMoneyImage", parent)->AddComponent<Image>();
    addMoneyImage->LoadTexture(0, 0, "UI/MoneyAddedBackground.png", -0.7f);
    addMoneyImage->SetPosition(0, 1080 - addMoneyImage->GetHeight());
    // Animator is added to image so that it's automatically destroyed after animation is done
    auto addMoneyAnimator = addMoneyImage->GetParent()->AddComponent<UIAnimator>();
    addMoneyAnimator->Setup(addMoneyImage, {
            {AnimatedProperty::Position, glm::vec3(220, 1080 - addMoneyImage->GetHeight(), 0), 0.2f},
            {AnimatedProperty::Position, glm::vec3(220, 1080 - addMoneyImage->GetHeight(), 0), 1.0f},
            {AnimatedProperty::Position, glm::vec3(0, 1080 - addMoneyImage->GetHeight(), 0), 0.5f}
    });
    auto addMoneyText = GameObject::Instantiate("AddMoneyText", parent)->AddComponent<Text>();
    addMoneyText->LoadFont((cashDiff>0?"+$":"") + std::to_string(cashDiff),
                           0, 0, 48);
    addMoneyText->SetPosition(0, 1080 - addMoneyImage->GetHeight()/2 - 16);
    addMoneyText->z = -0.75f;
    auto addMoneyTextAnimator = addMoneyText->GetParent()->AddComponent<UIAnimator>();
    addMoneyTextAnimator->Setup(addMoneyText, {
            {AnimatedProperty::Position, glm::vec3(250, 1080 - addMoneyImage->GetHeight()/2 - 16, 0), 0.2f},
            {AnimatedProperty::Position, glm::vec3(250, 1080 - addMoneyImage->GetHeight()/2 - 16, 0), 1.0f},
            {AnimatedProperty::Position, glm::vec3(0, 1080 - addMoneyImage->GetHeight()/2 - 16, 0), 0.5f}
    });
}

void PlayerUI::UpdateBadges(const std::map<PlayerBadges, bool>& badgesStates) {
    for(auto state : badgesStates) {
        if(state.second) badges[state.first]->SetAlpha(1);
    }
}

void PlayerUI::OnDestroy() {
    cashText.reset();
    Component::OnDestroy();
}
