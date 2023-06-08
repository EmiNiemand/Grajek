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
#include "EngineManagers/SavePointManager.h"
#include "Components/Animations/UIAnimator.h"
#include "LowLevelClasses/GameData.h"
#include <fstream>

//TODO: what the hell happened here o.0
PlayerUI::PlayerUI(const std::shared_ptr<GameObject> &parent, int id)
        : Component(parent, id) {
    cashText = GameObject::Instantiate("Money", parent)->AddComponent<Text>();
    cashText->LoadFont("$: 0", 20, 1010, 64, glm::vec3(1));
    cashText->z = -0.85;
    cashBackground =  GameObject::Instantiate("UI", parent)->AddComponent<Image>();
    cashBackground->LoadTexture(0, 0, "UI/MoneyBackground.png", -0.8);
    cashBackground->SetPosition(0, 1080 - cashBackground->GetHeight());

    auto menus = GameObject::Instantiate("Menus", parent);

    // Set up pause menu
    auto pause = GameObject::Instantiate("Pause", menus)->AddComponent<PauseMenu>();
    {
        std::vector<std::string> buttonNames = {
                "Resume",
                "Options",
                "Main Menu",
                "Exit"
        };
        std::vector<std::string> buttonImageNames = {
                "Resume",
                "Options",
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
                                                 "", 32, glm::vec3(), GameFont::KanitLight, -1);
            optionButton->ChangePosition(50, currentYPos);
            currentYPos += buttonOffset;

            buttonBuffer.push_back(optionButton);
        }

        pause->AddImage("Background", 0, 0, "UI/Pauza/PauzaBG.png", -0.95);

        for(int i=0; i<buttonNames.size(); i++) {
            auto prevIndex = i-1<0 ? buttonNames.size()-1:i-1;
            auto nextIndex = i+1==buttonNames.size() ? 0:i+1;
            buttonBuffer[i]->previousButton = buttonBuffer[prevIndex];
            buttonBuffer[i]->nextButton = buttonBuffer[nextIndex];
        }

        pause->GetParent()->DisableSelfAndChildren();
    }

    // Set up options menu
    auto options = GameObject::Instantiate("Options", menus)->AddComponent<OptionsMenu>();
    {
        auto backToPauseMenu = options->AddButton("BackToPauseMenu", 380, 870, "UI/Opcje/Guzik.png", "UI/Opcje/GuzikZRamka.png");
        auto musicVolume = options->AddButton("MusicVolume", 538, 600, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
        auto windowResolution = options->AddButton("WindowResolution", 790, 395, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
        auto windowFullScreen = options->AddButton("WindowFullScreen", 1041, 175, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
        auto shadowResolution = options->AddButton("ShadowResolution", 1296, 600, "UI/Opcje/Suwak.png", "UI/Opcje/SuwakZRamka.png");
        auto previousValue = options->AddButton("PreviousValue", 767, 882, "UI/Opcje/Tekst.png", "UI/Opcje/Suwak.png", "", 30);
        auto currentValue = options->AddButton("CurrentValue", 767, 845, "UI/Opcje/Tekst.png", "UI/Opcje/Suwak.png", "", 42);
        auto nextValue = options->AddButton("NextValue", 767, 808, "UI/Opcje/Tekst.png", "UI/Opcje/Suwak.png", "", 30);
        auto optionsBackground = options->AddImage("OptionsBackground", 285, 40, "UI/Opcje/Ustawienia.png");

        backToPauseMenu->previousButton = shadowResolution;
        backToPauseMenu->nextButton = musicVolume;
        musicVolume->previousButton = backToPauseMenu;
        musicVolume->nextButton = windowResolution;
        windowResolution->previousButton = musicVolume;
        windowResolution->nextButton = windowFullScreen;
        windowFullScreen->previousButton = windowResolution;
        windowFullScreen->nextButton = shadowResolution;
        shadowResolution->previousButton = windowFullScreen;
        shadowResolution->nextButton = backToPauseMenu;

        options->GetParent()->DisableSelfAndChildren();
    }

    // Set up shop menu
    auto shopMenu = GameObject::Instantiate("ShopMenu", menus);
    {
        auto shopMenuComponent = shopMenu->AddComponent<ShopMenu>();
        auto firstInstrumentCost = shopMenuComponent->Menu::AddText("FirstInstrumentCost", "Cost: 100", 320, 220, 32,glm::vec3(1.0f, 1.0f, 1.0f));
        auto secondInstrumentCost = shopMenuComponent->Menu::AddText("SecondInstrumentCost", "Cost: 500", 1600, 620, 32,glm::vec3(1.0f, 1.0f, 1.0f));
        auto thirdInstrumentCost = shopMenuComponent->Menu::AddText("ThirdInstrumentCost", "Cost: 1500", 1770, 350, 32,glm::vec3(1.0f, 1.0f, 1.0f));
        auto fourthInstrumentCost = shopMenuComponent->Menu::AddText("FourthInstrumentCost", "Cost: 5000", 1040, 900,32, glm::vec3(1.0f, 1.0f, 1.0f));
        auto firstInstrument = shopMenuComponent->Menu::AddButton("FirstInstrument", 10, 0, "UI/Sklep/Perkusja.png","UI/Sklep/PerkusjaZRamka.png");
        auto secondInstrument = shopMenuComponent->Menu::AddButton("SecondInstrument", 1425, 525, "UI/Sklep/Trabka.png","UI/Sklep/TrabkaZRamka.png");
        auto thirdInstrument = shopMenuComponent->Menu::AddButton("ThirdInstrument", 1525, 250,"UI/Sklep/LaunbhPad.png","UI/Sklep/LaunbhPadZRamka.png");
        auto fourthInstrument = shopMenuComponent->Menu::AddButton("FourthInstrument", 600, 700, "UI/Sklep/Gitara.png","UI/Sklep/GitaraZRamka.png");
        auto exitButton = shopMenuComponent->Menu::AddImage("ExitImage", 1600, 50, "UI/Sklep/Przycisk2.png");
        auto shopBackground = shopMenuComponent->Menu::AddImage("ShopBackground", 0, 0, "UI/Sklep/Sklep.png");
        firstInstrumentCost->GetParent()->SetParent(firstInstrument->GetParent());
        secondInstrumentCost->GetParent()->SetParent(secondInstrument->GetParent());
        thirdInstrumentCost->GetParent()->SetParent(thirdInstrument->GetParent());
        fourthInstrumentCost->GetParent()->SetParent(fourthInstrument->GetParent());
        firstInstrument->previousButton = thirdInstrument;
        firstInstrument->nextButton = fourthInstrument;
        secondInstrument->previousButton = fourthInstrument;
        secondInstrument->nextButton = thirdInstrument;
        thirdInstrument->previousButton = secondInstrument;
        thirdInstrument->nextButton = firstInstrument;
        fourthInstrument->previousButton = firstInstrument;
        fourthInstrument->nextButton = secondInstrument;
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
                        "UI/buttonSaveInactive.png", "UI/buttonSaveActive.png", "Save " + currentIndex, 0, glm::vec3(1)));

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
                savePointMenu->saveDates.push_back(savePointMenu->Menu::AddText("SaveDate" + currentIndex, gameData->saveDate, xpos + 15, ypos + 125, 30));

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
        savePointMenu->AddImage("SavePointMenuBackground", 0, 0, "UI/pause.png");
        savePointMenu->buttonImage = savePointMenu->AddImage("SavePointMenuButtonImage", 1600, 50, "UI/enterSavePoint.png");
        SavePointManager::GetInstance()->buttonImage = savePointMenu->buttonImage;
        savePointMenu->GetParent()->DisableSelfAndChildren();
    }
}

void PlayerUI::OnDestroy() {
    cashText.reset();
    Component::OnDestroy();
}

void PlayerUI::UpdateCash(int newAmount, bool playAnimation) {
	cashText->text = "$ " + std::to_string(newAmount);

    if(!playAnimation) return;

    auto addMoneyImage = GameObject::Instantiate("AddMoneyImage", parent)->AddComponent<Image>();
    addMoneyImage->LoadTexture(0, 0, "UI/MoneyAddedBackground.png", -1);
    addMoneyImage->SetPosition(0, 1080 - addMoneyImage->GetHeight());
    // Animator is added to image so that it's automatically destroyed after animation is done
    auto addMoneyAnimator = addMoneyImage->GetParent()->AddComponent<UIAnimator>();
    addMoneyAnimator->Setup(addMoneyImage, {
            {AnimatedProperty::Position, glm::vec3(220, 1080 - addMoneyImage->GetHeight(), 0), 0.1f},
            {AnimatedProperty::Position, glm::vec3(220, 1080 - addMoneyImage->GetHeight(), 0), 1.0f},
            {AnimatedProperty::Position, glm::vec3(0, 1080 - addMoneyImage->GetHeight(), 0), 0.5f}
    });
    //TODO: add text animation when it gets implemented
}
