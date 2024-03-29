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
    shop->AddComponent<Renderer>()->LoadModel("Buildings/Normal/MAINSHOP.obj");
    shop->AddComponent<BoxCollider>()->SetOffset({-2.5, 3, 2});
    shop->GetComponent<BoxCollider>()->SetSize({2.5, 6, 2});

    auto shopTrigger = GameObject::Instantiate("ShopTrigger", shop);
    shopTrigger->AddComponent<BoxCollider>()->SetOffset({-2.5, 1, 4});
    shopTrigger->GetComponent<BoxCollider>()->SetSize({1.5, 1.5, 1.5});
    shopTrigger->GetComponent<BoxCollider>()->isTrigger = true;
    auto shopTriggerComponent = shopTrigger->AddComponent<ShopTrigger>();

    auto interactButton = GameObject::Instantiate("InteractButton", shopTrigger);
    interactButton->transform->SetLocalPosition({-2.5, 3, 2.5});
    auto interactButtonImage = GameObject::Instantiate("InteractButton", interactButton);
    auto interactButtonImageComponent = interactButtonImage->AddComponent<Image>();
    interactButtonImageComponent->LoadTexture(0, 0, "UI/Sklep/Przycisk.png", 0.5f);
    interactButtonImageComponent->isDynamic = true;
    interactButtonImageComponent->SetScale(0.5);
    interactButtonImageComponent->enabled = false;

    shopTriggerComponent->buttonImage = interactButtonImageComponent;

    return shop;
}

std::shared_ptr<StaticObjectData> Shop::SaveStatic() {
    std::shared_ptr<StaticObjectData> newData = std::make_shared<StaticObjectData>();
    std::shared_ptr<Renderer> objectRenderer = GetComponent<Renderer>();
    std::shared_ptr<BoxCollider> colider = GetComponent<BoxCollider>();

    newData->name = this->GetPrefabName();
    newData->uniqueName = this->GetName();
    newData->position = this->transform->GetLocalPosition();
    newData->rotation = this->transform->GetLocalRotation();
    newData->scale = this->transform->GetLocalScale();
    newData->modelPath = objectRenderer->lastLoadedModelPath;
    newData->coliderSize = colider->GetSize();
    newData->coliderOffset = colider->GetOffset();

    return newData;
}
