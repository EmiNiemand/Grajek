#include "Components/Scripts/ShopMenu.h"
#include "GloomEngine.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/UI/Button.h"

ShopMenu::ShopMenu(const std::shared_ptr<GameObject> &parent, int id) : Menu(parent, id) {}

ShopMenu::~ShopMenu() {}

void ShopMenu::ShowMenu() {
    parent->EnableSelfAndChildren();
    activeButton = GloomEngine::GetInstance()->FindGameObjectWithName("FourthInstrument")->GetComponent<Button>();
    activeButton->isActive = true;
}

void ShopMenu::OnClick() {
    if (GloomEngine::GetInstance()->FindGameObjectWithName("FirstInstrument")->GetComponent<Button>()->isActive) {
        // TODO buy first instrument
    }
}
