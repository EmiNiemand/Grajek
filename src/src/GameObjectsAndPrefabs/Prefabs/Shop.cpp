#include "GameObjectsAndPrefabs/Prefabs/Shop.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/Menus/ShopMenu.h"
#include "Components/Scripts/Menus/ShopTrigger.h"
#include "Components/UI/Button.h"
#include "Components/UI/Image.h"
#include "Components/Scripts/Menus/Shopkeeper.h"

Shop::Shop(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
           Prefab(name, id, parent, tag) {
    prefabName = "Shop";
}

Shop::~Shop() = default;

std::shared_ptr<GameObject> Shop::Create() {
    auto shop = shared_from_this();
    shop->transform->SetLocalPosition({4, 0, -8});
    shop->transform->SetLocalRotation({0, 0, 0});
    shop->transform->SetLocalScale({1, 1, 1});
    shop->AddComponent<Renderer>()->LoadModel("Buildings/MAINSHOP.obj");
    shop->AddComponent<BoxCollider>()->SetOffset({-2.5, 3, 2});
    shop->GetComponent<BoxCollider>()->SetSize({2.5, 6, 2});
    auto shopkeeper = GameObject::Instantiate("Shopkeeper", shop);
    shopkeeper->AddComponent<Shopkeeper>();

    auto shopMenu = GameObject::Instantiate("ShopMenu", shop);
    auto shopMenuComponent = shopMenu->AddComponent<ShopMenu>();
    auto firstInstrumentCost = shopMenuComponent->Menu::AddText("FirstInstrumentCost", "Cost: 100", 320, 220, 32, glm::vec3(1.0f, 1.0f, 1.0f));
    auto secondInstrumentCost = shopMenuComponent->Menu::AddText("SecondInstrumentCost", "Cost: 500", 1600, 620, 32, glm::vec3(1.0f, 1.0f, 1.0f));
    auto thirdInstrumentCost = shopMenuComponent->Menu::AddText("ThirdInstrumentCost", "Cost: 1500", 1770, 350, 32, glm::vec3(1.0f, 1.0f, 1.0f));
    auto fourthInstrumentCost = shopMenuComponent->Menu::AddText("FourthInstrumentCost", "Cost: 5000", 1040, 900, 32, glm::vec3(1.0f, 1.0f, 1.0f));
    auto firstInstrument = shopMenuComponent->Menu::AddButton("FirstInstrument", 10, 0, "UI/Sklep/Perkusja.png", "UI/Sklep/PerkusjaZRamka.png");
    auto secondInstrument = shopMenuComponent->Menu::AddButton("SecondInstrument", 1425, 525, "UI/Sklep/Trabka.png", "UI/Sklep/TrabkaZRamka.png");
    auto thirdInstrument = shopMenuComponent->Menu::AddButton("ThirdInstrument", 1525, 250, "UI/Sklep/LaunbhPad.png", "UI/Sklep/LaunbhPadZRamka.png");
    auto fourthInstrument = shopMenuComponent->Menu::AddButton("FourthInstrument", 600, 700, "UI/Sklep/Gitara.png", "UI/Sklep/GitaraZRamka.png");
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

    auto shopTrigger = GameObject::Instantiate("ShopTrigger", shop);
    shopTrigger->AddComponent<BoxCollider>()->SetOffset({-2.5, 1, 4});
    shopTrigger->GetComponent<BoxCollider>()->SetSize({1.5, 1.5, 1.5});
    shopTrigger->GetComponent<BoxCollider>()->isTrigger = true;
    shopTrigger->AddComponent<ShopTrigger>();
    GameObject::Instantiate("ShopButton", shopTrigger)->AddComponent<Image>()->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png");
    shopTrigger->children.begin()->second->DisableSelfAndChildren();

    return shop;
}
