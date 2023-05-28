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

//TODO: what the hell happened here o.0
PlayerUI::PlayerUI(const std::shared_ptr<GameObject> &parent, int id)
        : Component(parent, id) {
    cashText = GameObject::Instantiate("Money", parent)->AddComponent<Text>();
    cashText->LoadFont("Money: 0", 140, 1040, 22);
    repText = GameObject::Instantiate("Reputation", parent)->AddComponent<Text>();
    repText->LoadFont("Rep: 0", 140, 1000, 22);
    GameObject::Instantiate("UI", parent)->AddComponent<Image>()->LoadTexture(0, 952, "UI/Player.png");

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

        int buttonOffset = -100;
        int currentYPos = 540 - buttonOffset*(buttonNames.size()-2)/2;

        std::vector<std::shared_ptr<Button>> buttonBuffer;

        for (int i = 0; i < buttonNames.size(); ++i) {
            auto optionButton = pause->AddButton(buttonNames[i]+"Button", 0, 0,
                                                 "UI/buttonInactive.png", "UI/buttonActive.png",
                                                 buttonNames[i], 32);
            optionButton->ChangePosition(960 - optionButton->GetWidth()/2, currentYPos - optionButton->GetHeight()/2);
            currentYPos += buttonOffset;

            buttonBuffer.push_back(optionButton);
        }

        pause->AddImage("Background", 0, 0, "UI/pause.png");

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
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 5; j++) {
                savePointMenu->Menu::AddButton("Save" + std::to_string(i * 5 + j + 1), j * 300 + 50 * (j + 1),
                                               i * 300 + 100 * (i + 1), "UI/buttonInactive.png", "UI/buttonActive.png",
                                               "Save " + std::to_string(i * 5 + j + 1), 32);
            }
        }
        GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save1")->GetComponent<Button>()->previousButton = GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save10")->GetComponent<Button>();
        GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save1")->GetComponent<Button>()->nextButton = GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save2")->GetComponent<Button>();
        for (int i = 2; i <= 9; i++) {
            GloomEngine::GetInstance()->FindGameObjectWithName("Save" + std::to_string(
                    i))->GetComponent<Button>()->previousButton = GloomEngine::GetInstance()->FindGameObjectWithName(
                    "Save" + std::to_string(i - 1))->GetComponent<Button>();
            GloomEngine::GetInstance()->FindGameObjectWithName("Save" + std::to_string(
                    i))->GetComponent<Button>()->nextButton = GloomEngine::GetInstance()->FindGameObjectWithName(
                    "Save" + std::to_string(i + 1))->GetComponent<Button>();
        }
        GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save10")->GetComponent<Button>()->previousButton = GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save9")->GetComponent<Button>();
        GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save10")->GetComponent<Button>()->nextButton = GloomEngine::GetInstance()->FindGameObjectWithName(
                "Save1")->GetComponent<Button>();
        savePointMenu->AddImage("SavePointMenuBackground", 0, 0, "UI/pause.png");
        savePointMenu->buttonImage = savePointMenu->AddImage("SavePointMenuButtonImage", 1600, 50, "UI/Sklep/Przycisk.png");
        savePointMenu->GetParent()->DisableSelfAndChildren();
        SavePointManager::GetInstance()->buttonImage = savePointMenu->buttonImage;
    }
}

void PlayerUI::UpdateCash(int newAmount) {
	cashText->text = "Money: " + std::to_string(newAmount);
}

void PlayerUI::UpdateRep(int newAmount) {
	repText->text = "Rep: " + std::to_string(newAmount);
}