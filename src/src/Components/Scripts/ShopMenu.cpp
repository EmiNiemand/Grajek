#include "Components/Scripts/ShopMenu.h"
#include "Components/Scripts/PlayerManager.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/UI/Button.h"
#include "Components/Scripts/ShopTrigger.h"

ShopMenu::ShopMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

ShopMenu::~ShopMenu() {}

void ShopMenu::Start() {
    Component::Start();
    // Remove already bought instruments from shop
    // TODO: improve
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    auto playerInstruments = playerManager->GetInstruments();
    if(playerInstruments.contains(InstrumentName::Drums))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>());
    if(playerInstruments.contains(InstrumentName::Trumpet))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("SecondInstrument")->GetComponent<Button>());
    if(playerInstruments.contains(InstrumentName::Launchpad))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("ThirdInstrument")->GetComponent<Button>());
    if(playerInstruments.contains(InstrumentName::Guitar))
        DeleteButton(GloomEngine::GetInstance()->FindGameObjectWithName("FourthInstrument")->GetComponent<Button>());
}

bool ShopMenu::ShowMenu() {
    if (!GloomEngine::GetInstance()->FindGameObjectWithName("ShopTrigger")->GetComponent<ShopTrigger>()->active) return false;
    parent->EnableSelfAndChildren();
    if (GloomEngine::GetInstance()->FindGameObjectWithName("ShopMenu")->children.size() > 1) {
        activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("ShopMenu")->children.begin()->second->GetComponent<Button>();
        activeButton->isActive = true;
    }
    return true;
}

void ShopMenu::OnClick() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("ShopMenu")->children.size() < 2) return;
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    bool boughtInstrument = false;
    if (activeButton->GetParent()->GetName() == "FirstInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                100, Prefab::GetInstrument(InstrumentName::Drums));
    } else if (activeButton->GetParent()->GetName() == "SecondInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                500, Prefab::GetInstrument(InstrumentName::Trumpet));
    } else if (activeButton->GetParent()->GetName() == "ThirdInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                1500, Prefab::GetInstrument(InstrumentName::Launchpad));
    } else if (activeButton->GetParent()->GetName() == "FourthInstrument") {
        boughtInstrument = playerManager->BuyInstrument(
                5000, Prefab::GetInstrument(InstrumentName::Guitar));
    }
    if(boughtInstrument) {
        DeleteButton(activeButton);
        spdlog::info("[SM] Bought instrument!");
    } else {
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