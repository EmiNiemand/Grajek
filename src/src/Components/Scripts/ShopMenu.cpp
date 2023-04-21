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
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("FourthInstrument")->GetComponent<Button>();
    activeButton->isActive = true;
}

void ShopMenu::OnClick() {
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
        // TODO delete button with instrument
        std::shared_ptr<Button> temp = activeButton->previousButton;
        activeButton->previousButton->nextButton = activeButton->nextButton;
        activeButton->nextButton->previousButton = temp;
        GameObject::Destroy(activeButton->GetParent());
//            GameObject::Destroy(GloomEngine::GetInstance()->FindGameObjectWithName("ui"));
        activeButton = activeButton->nextButton;
        spdlog::info("[SM] Bought instrument!");
    } else {
        spdlog::info("[SM] Not enough money for instrument");
    }
}
