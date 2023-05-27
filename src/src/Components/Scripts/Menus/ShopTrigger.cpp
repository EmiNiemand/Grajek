#include "Components/Scripts/Menus/ShopTrigger.h"
#include "GameObjectsAndPrefabs/GameObject.h"
#include "Components/Animations/GameObjectAnimator.h"
#include "Components/Renderers/Renderer.h"

ShopTrigger::ShopTrigger(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

ShopTrigger::~ShopTrigger() = default;

void ShopTrigger::Start() {
    door = GameObject::Instantiate("Door", parent->parent);
    door->AddComponent<Renderer>()->LoadModel("Buildings/drzwi.obj");
    door->transform->SetLocalPosition(glm::vec3(-3, 0, 3.5f));
    Component::Start();
}

void ShopTrigger::OnTriggerEnter(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    auto animation = GameObject::Instantiate("DoorAnimation", door);
    animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, 60.0f, 0.0f), 1.5f}
    }, false);
    active = true;
    buttonImage->enabled = true;
    Component::OnTriggerEnter(gameObject);
}

void ShopTrigger::OnTriggerExit(const std::shared_ptr<GameObject> &gameObject) {
    if (gameObject->GetName() != "Player") return;
    auto animation = GameObject::Instantiate("DoorAnimation", door);
    animation->AddComponent<GameObjectAnimator>()->Setup(door->transform, {
            {AnimatedProperty::Rotation, glm::vec3(0.0f, -60.0f, 0.0f), 1.5f}
    }, false);
    active = false;
    buttonImage->enabled = false;
    Component::OnTriggerExit(gameObject);
}