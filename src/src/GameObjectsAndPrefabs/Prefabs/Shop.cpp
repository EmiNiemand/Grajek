#include "GameObjectsAndPrefabs/Prefabs/Shop.h"
#include "Components/Renderers/Renderer.h"
#include "Components/PhysicsAndColliders/BoxCollider.h"
#include "Components/Scripts/ShopMenu.h"
#include "Components/Scripts/ShopTrigger.h"
#include "Components/UI/Button.h"
#include "Components/UI/Image.h"

Shop::Shop(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
           Prefab(name, id, parent, tag) {
    prefabName = "Shop";
}

Shop::~Shop() = default;

std::shared_ptr<GameObject> Shop::Create() {
    auto shop = shared_from_this();
    shop->transform->SetLocalPosition({-10, 0, -5});
    shop->transform->SetLocalRotation({0, -45, 0});
    shop->transform->SetLocalScale({0.7, 0.7, 0.7});
    shop->AddComponent<Renderer>()->LoadModel("texturedModels/stoisko.obj");
    shop->AddComponent<BoxCollider>()->SetOffset({0, 2, -9});
    shop->GetComponent<BoxCollider>()->SetSize({1, 3, 3});

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
    std::shared_ptr<GameObject> exitButton = shopMenuComponent->Menu::AddImage("ExitButton", 1600, 50, "UI/Sklep/Przycisk2.png");
    std::shared_ptr<GameObject> shopBackground = shopMenuComponent->Menu::AddImage("ShopBackground", 0, 0, "UI/Sklep/Sklep.png");
    firstInstrumentCost->SetParent(firstInstrument);
    secondInstrumentCost->SetParent(secondInstrument);
    thirdInstrumentCost->SetParent(thirdInstrument);
    fourthInstrumentCost->SetParent(fourthInstrument);
    firstInstrument->GetComponent<Button>()->previousButton = thirdInstrument->GetComponent<Button>();
    firstInstrument->GetComponent<Button>()->nextButton = fourthInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->previousButton = fourthInstrument->GetComponent<Button>();
    secondInstrument->GetComponent<Button>()->nextButton = thirdInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->previousButton = secondInstrument->GetComponent<Button>();
    thirdInstrument->GetComponent<Button>()->nextButton = firstInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->previousButton = firstInstrument->GetComponent<Button>();
    fourthInstrument->GetComponent<Button>()->nextButton = secondInstrument->GetComponent<Button>();
    shopMenu->DisableSelfAndChildren();

    auto shopTrigger = GameObject::Instantiate("ShopTrigger", shop);
    shopTrigger->AddComponent<BoxCollider>()->SetOffset({0, 2, -9});
    shopTrigger->GetComponent<BoxCollider>()->SetSize({4, 6, 6});
    shopTrigger->GetComponent<BoxCollider>()->isTrigger = true;
    shopTrigger->AddComponent<ShopTrigger>();
    GameObject::Instantiate("ShopButton", shopTrigger)->AddComponent<Image>()->LoadTexture(1600, 50, "UI/Sklep/Przycisk.png");
    shopTrigger->children.begin()->second->DisableSelfAndChildren();

    return shop;
}
