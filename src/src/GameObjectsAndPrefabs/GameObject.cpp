#include "include/GameObjectsAndPrefabs/GameObject.h"
#include "include/GloomEngine.h"
#include "include/Components/Component.h"

std::shared_ptr<GloomEngine> GameObject::gloomEngine = nullptr;
std::shared_ptr<GameObjectFactory> GameObject::gameObjectFactory = nullptr;
std::shared_ptr<ComponentFactory> GameObject::componentFactory = nullptr;

GameObject::GameObject(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
                                                                        name(name), id(id), parent(parent), tag(tag) {}

GameObject::~GameObject() {}

std::shared_ptr<GameObject> GameObject::Instantiate(std::string name, std::shared_ptr<GameObject> parent, Tags tag) {
    return gameObjectFactory->CreateGameObject(name, parent, tag);
}

void GameObject::Destroy(std::shared_ptr<GameObject> gameObject) {
    gameObject->parent->RemoveChild(gameObject->GetId());
}

std::shared_ptr<Component> GameObject::GetComponentByName(ComponentNames name) {
    for (auto&& component : components) {
        if (component.second->GetName() == name) return component.second;
    }
    return nullptr;
}

void GameObject::OnTransformUpdateComponents() {
    for (auto&& component : components) {
        component.second->OnUpdate();
    }
}

void GameObject::RemoveComponent(int componentId) {
    if (!components.contains(componentId)) return;
    gloomEngine->RemoveComponent(components.find(componentId)->second);
    components.erase(componentId);
}

void GameObject::RemoveAllComponents() {
    for (auto&& component : components){
        gloomEngine->RemoveComponent(component.second);
    }
    components.clear();
}

void GameObject::SetParent(const std::shared_ptr<GameObject> &newParent) {
    parent = newParent;
}

void GameObject::AddChild(const std::shared_ptr<GameObject> &child) {
    child->parent = shared_from_this();
    children.insert({child->GetId(), child});
}

void GameObject::RemoveChild(int childId) {
    if (!children.contains(childId)) return;
    children.find(childId)->second->RemoveAllChildren();
    children.erase(childId);
}

void GameObject::RemoveAllChildren() {
    for (auto&& child : children) {
        child.second->RemoveAllComponents();
        child.second->RemoveAllChildren();
        gloomEngine->RemoveGameObject(child.second);
    }
    children.clear();
}

void GameObject::UpdateSelfAndChildren() {
    if (transform != nullptr) {
        OnTransformUpdateComponents();
        ForceUpdateSelfAndChildren();
    }
}

void GameObject::ForceUpdateSelfAndChildren() {
    if (parent != nullptr) transform->ComputeModelMatrix(parent->transform->GetModelMatrix());
    else transform->ComputeModelMatrix();

    for (auto&& child : children)
    {
        child.second->ForceUpdateSelfAndChildren();
        child.second->OnTransformUpdateComponents();
    }
}

int GameObject::GetId() const {
    return id;
}

const std::string &GameObject::GetName() const {
    return name;
}

void GameObject::Init(const std::shared_ptr<GloomEngine> &gloomEngine) {
    GameObject::gloomEngine = gloomEngine;
    GameObject::gameObjectFactory = std::make_shared<GameObjectFactory>(gloomEngine);
    GameObject::componentFactory = std::make_shared<ComponentFactory>(gloomEngine);
}

void GameObject::AddComponentToList(std::shared_ptr<Component> component) {
    components.insert({component->GetId(), component});
}
