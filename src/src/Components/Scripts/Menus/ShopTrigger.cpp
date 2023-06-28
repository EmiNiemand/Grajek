#include "Components/Scripts/Menus/ShopTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Animations/GameObjectAnimator.h"
#include "Components/Renderers/Renderer.h"
#include "Components/Scripts/Menus/ShopMenu.h"
#include "GloomEngine.h"
#include "EngineManagers/DialogueManager.h"

ShopTrigger::ShopTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

ShopTrigger::~ShopTrigger() = default;

void ShopTrigger::Start() {
    door = GameObject::Instantiate("Door", parent->parent);
    door->AddComponent<Renderer>()->LoadModel("Buildings/Normal/drzwi.obj");
    door->transform->SetLocalPosition(glm::vec3(-3, 0, 3.5f));
    shopMenu = GloomEngine::GetInstance()->FindGameObjectWithName("ShopMenu")->GetComponent<ShopMenu>();
    Component::Start();
}

void ShopTrigger::OnDestroy() {
    door.reset();
    buttonImage.reset();
    Component::OnDestroy();
}

void ShopTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    auto animation = GameObject::Instantiate("DoorAnimation", door);
    animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, 60.0f, 0.0f), 1.5f}
    }, false);
    shopMenu->triggerActive = true;
    buttonImage->enabled = true;
    DialogueManager::GetInstance()->shopTrigger = std::dynamic_pointer_cast<ShopTrigger>(shared_from_this());
    Component::OnTriggerEnter(gameObject);
}

void ShopTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    auto animation = GameObject::Instantiate("DoorAnimation", door);
    animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, -60.0f, 0.0f), 1.5f}
    }, false);
    shopMenu->triggerActive = false;
    buttonImage->enabled = false;
    DialogueManager::GetInstance()->shopTrigger.reset();
    Component::OnTriggerExit(gameObject);
}
