#include "Components/Scripts/ShopMenu.h"
#include "Components/Scripts/PlayerManager.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/UI/Button.h"

ShopMenu::ShopMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

ShopMenu::~ShopMenu() {}

void ShopMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    if (GloomEngine::GetInstance()->FindGameObjectWithName("Shop")->children.size() > 1) {
        activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("Shop")->children.begin()->second->GetComponent<Button>();
        activeButton->isActive = true;
    }
}

void ShopMenu::OnClick() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("Shop")->children.size() < 2) return;
    bool bought;
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    if (activeButton->GetParent()->GetName() == "FirstInstrument") {
        if (playerManager->BuyInstrument(10, Prefab::GetInstrument(InstrumentName::Drums)))
            bought = true;
        else
            bought = false;
    } else if (activeButton->GetParent()->GetName() == "SecondInstrument") {
        if (playerManager->BuyInstrument(15, Prefab::GetInstrument(InstrumentName::Trumpet)))
            bought = true;
        else
            bought = false;
    } else if (activeButton->GetParent()->GetName() == "ThirdInstrument") {
        if (playerManager->BuyInstrument(20, Prefab::GetInstrument(InstrumentName::Launchpad)))
            bought = true;
        else
            bought = false;
    } else if (activeButton->GetParent()->GetName() == "FourthInstrument") {
        if (playerManager->BuyInstrument(25, Prefab::GetInstrument(InstrumentName::Guitar)))
            bought = true;
        else
            bought = false;
    }

    if (bought) {
        spdlog::info("[SM] Bought first instrument (percussion)!");
        std::shared_ptr<Button> temp = activeButton->previousButton;
        activeButton->previousButton->nextButton = activeButton->nextButton;
        activeButton->nextButton->previousButton = temp;
        GameObject::Destroy(activeButton->GetParent());
        activeButton = temp;
        activeButton->isActive = true;
    } else {
        spdlog::info("[SM] Not enough money for first instrument (percussion)");
    }
}
