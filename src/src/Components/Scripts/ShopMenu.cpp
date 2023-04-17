#include "Components/Scripts/ShopMenu.h"
#include "Components/Scripts/PlayerManager.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "GameObjectsAndPrefabs/Prefab.h"
#include "Components/UI/Button.h"

ShopMenu::ShopMenu(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

ShopMenu::~ShopMenu() {}

void ShopMenu::Start() {
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("FourthInstrument")->GetComponent<Button>();
    Component::Start();
}

void ShopMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("FourthInstrument")->GetComponent<Button>();
    activeButton->isActive = true;
}

void ShopMenu::HideMenu() {
    parent->DisableSelfAndChildren();
    activeButton->isActive = false;
}

void ShopMenu::ChangeActiveButton(glm::vec2 moveVector) {
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
}

void ShopMenu::OnClick() {
    auto playerManager = GloomEngine::GetInstance()->FindGameObjectWithName("Player")->GetComponent<PlayerManager>();
    if (GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>()->isActive) {
        if(playerManager->BuyInstrument(10, Prefab::GetInstrument(InstrumentName::Drums)))
            spdlog::info("[SM] Bought first instrument (percussion)!");
        else
            spdlog::info("[SM] Not enough money for first instrument (percussion)");
    }
}
