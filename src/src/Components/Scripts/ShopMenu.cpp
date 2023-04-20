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
    if (activeButton->GetParent()->GetName() == "FirstInstrument") {
        if(playerManager->BuyInstrument(10, Prefab::GetInstrument(InstrumentName::Drums))) {
            // TODO delete button with instrument
            std::shared_ptr<Button> temp = activeButton->previousButton;
            activeButton->previousButton->nextButton = activeButton->nextButton;
            activeButton->nextButton->previousButton = temp;
            GameObject::Destroy(activeButton->GetParent());
            activeButton->GetParent();
//            GameObject::Destroy(GloomEngine::GetInstance()->FindGameObjectWithName("ui"));
            activeButton = activeButton->nextButton;
            spdlog::info("[SM] Bought first instrument (percussion)!");
        } else {
            spdlog::info("[SM] Not enough money for first instrument (percussion)");
        }
    }
}
