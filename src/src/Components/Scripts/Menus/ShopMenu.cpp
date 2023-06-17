#include "Components/Scripts/Menus/ShopMenu.h"
#include "Components/Scripts/Player/PlayerManager.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Scripts/Instrument.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/Menus/ShopTrigger.h"
#include "Components/UI/Popup.h"
#include "Components/UI/Image.h"

ShopMenu::ShopMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

ShopMenu::~ShopMenu() = default;

void ShopMenu::Awake() {
    // Remove already bought instruments from shop
    // TODO: improve
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    auto playerInstruments = playerManager->GetInstruments();
    if(playerInstruments.contains(InstrumentName::Drums))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>());
    else {
        instruments.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>());
        secondInstrumentIndex++;
        thirdInstrumentIndex++;
        fourthInstrumentIndex++;
    }
    if(playerInstruments.contains(InstrumentName::Trumpet))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("SecondInstrument")->GetComponent<Button>());
    else {
        instruments.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("SecondInstrument")->GetComponent<Button>());
        thirdInstrumentIndex++;
        fourthInstrumentIndex++;
    }
    if(playerInstruments.contains(InstrumentName::Launchpad))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("ThirdInstrument")->GetComponent<Button>());
    else {
        instruments.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("ThirdInstrument")->GetComponent<Button>());
        fourthInstrumentIndex++;
    }
    if(playerInstruments.contains(InstrumentName::Guitar))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("FourthInstrument")->GetComponent<Button>());
    else
        instruments.push_back(GloomEngine::GetInstance()->FindGameObjectWithName("FourthInstrument")->GetComponent<Button>());

    sound = parent->AddComponent<AudioSource>();
    sound->LoadAudioData("res/sounds/direct/shop.wav", AudioType::Direct);
    sound->SetGain(0);
    sound->IsLooping(true);
    sound->PlaySound();
    Component::Awake();
}

void ShopMenu::OnDestroy() {
    instruments.clear();
    Menu::OnDestroy();
}

void ShopMenu::ChangeActiveButton(glm::vec2 moveVector) {
    activeButton->GetParent()->children.begin()->second->DisableSelfAndChildren();
    if (moveVector.y == 1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->previousButton;
        activeButton->isActive = true;
    }
    if (moveVector.y == -1.0f) {
        activeButton->isActive = false;
        activeButton = activeButton->nextButton;
        activeButton->isActive = true;
    }
    activeButton->GetParent()->children.begin()->second->EnableSelfAndChildren();
}

bool ShopMenu::ShowMenu() {
    if (!triggerActive) return false;
    if(!Menu::ShowMenu()) return false;

    sound->SetGain(1);
    GloomEngine::GetInstance()->timeScale = 1;

    if (!instruments.empty()) {
        for (const auto & instrument : instruments) {
            instrument->GetParent()->children.begin()->second->DisableSelfAndChildren();
        }
        activeButton = instruments[0];
        activeButton->isActive = true;
        activeButton->GetParent()->children.begin()->second->EnableSelfAndChildren();
    } else {
        buyImage->enabled = false;
    }

    return true;
}

void ShopMenu::OnClick() {
    if (instruments.empty()) return;
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    bool boughtInstrument = false;
    if (activeButton->GetParent()->GetName() == "FirstInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                100, Instrument::GetInstrument(InstrumentName::Drums));
    } else if (activeButton->GetParent()->GetName() == "SecondInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                500, Instrument::GetInstrument(InstrumentName::Trumpet));
    } else if (activeButton->GetParent()->GetName() == "ThirdInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                1500, Instrument::GetInstrument(InstrumentName::Launchpad));
    } else if (activeButton->GetParent()->GetName() == "FourthInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                5000, Instrument::GetInstrument(InstrumentName::Guitar));
    }
    if(boughtInstrument) {
        if (activeButton->GetParent()->GetName() == "FirstInstrument"){
            instruments.erase(instruments.begin());
            secondInstrumentIndex--;
            thirdInstrumentIndex--;
            fourthInstrumentIndex--;
            GameObject::Instantiate("Popup", parent)->AddComponent<Popup>()->
                    Setup(610, 340, "UI/Sklep/Popup.png", "UI/buttonInactive.png", "UI/buttonActive.png");
        } else if (activeButton->GetParent()->GetName() == "SecondInstrument") {
            instruments.erase(instruments.begin() + secondInstrumentIndex);
            thirdInstrumentIndex--;
            fourthInstrumentIndex--;
            GameObject::Instantiate("Popup", parent)->AddComponent<Popup>()->
                    Setup(610, 340, "UI/Sklep/Popup.png", "UI/buttonInactive.png", "UI/buttonActive.png");
        } else if (activeButton->GetParent()->GetName() == "ThirdInstrument") {
            instruments.erase(instruments.begin() + thirdInstrumentIndex);
            fourthInstrumentIndex--;
            GameObject::Instantiate("Popup", parent)->AddComponent<Popup>()->
                    Setup(610, 340, "UI/Sklep/Popup.png", "UI/buttonInactive.png", "UI/buttonActive.png");
        } else if (activeButton->GetParent()->GetName() == "FourthInstrument") {
            instruments.erase(instruments.begin() + fourthInstrumentIndex);
            GameObject::Instantiate("Popup", parent)->AddComponent<Popup>()->
                    Setup(610, 340, "UI/Sklep/Popup.png", "UI/buttonInactive.png", "UI/buttonActive.png");
        }
        DeleteButton(activeButton);
        if (instruments.empty()) buyImage->enabled = false;
        spdlog::info("[SM] Bought instrument!");
    } else {
        GameObject::Instantiate("Popup", parent)->AddComponent<Popup>()->
                Setup(610, 340, "UI/Sklep/Cash.png", "UI/buttonInactive.png", "UI/buttonActive.png");
        spdlog::info("[SM] Not enough money for instrument");
    }
}

void ShopMenu::DeleteButton(std::shared_ptr<Button> button) {
    if(button == activeButton)
        ChangeActiveButton({0, -1});
    std::shared_ptr<Button> temp = button->previousButton;
    button->previousButton->nextButton = button->nextButton;
    button->nextButton->previousButton = temp;
    GameObject::Destroy(button->GetParent());
}

void ShopMenu::HideMenu() {
    Menu::HideMenu();
    sound->SetGain(0);
}