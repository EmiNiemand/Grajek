#include "Components/Scripts/Menus/ShopTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Animations/GameObjectAnimator.h"

ShopTrigger::ShopTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

ShopTrigger::~ShopTrigger() = default;

void ShopTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    auto door = GloomEngine::GetInstance()->FindGameObjectWithName("Door");
    auto animation = GameObject::Instantiate("DoorAnimation", door);
    animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, 60.0f, 0.0f), 1.5f}
    }, false);
    active = true;
    parent->children.begin()->second->EnableSelfAndChildren();
    Component::OnTriggerEnter(gameObject);
}

void ShopTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    // TODO/URGENT don't search for object by name, replace with pointer
    auto door = GloomEngine::GetInstance()->FindGameObjectWithName("Door");
    auto animation = GameObject::Instantiate("DoorAnimation", door);
    animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, -60.0f, 0.0f), 1.5f}
    }, false);
    active = false;
    parent->children.begin()->second->DisableSelfAndChildren();
    Component::OnTriggerExit(gameObject);
}