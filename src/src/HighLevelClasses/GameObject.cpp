//
// Created by szymo on 01/03/2023.
//

#include "include/HighLevelClasses/GameObject.h"
#include "include/GloomEngine.h"
#include "include/Components/Component.h"

GameObject::GameObject(const std::shared_ptr<GloomEngine> &gloomEngine, const std::string &name, int id,
                       const std::shared_ptr<GameObject> &parent, Tags tag) : gloomEngine(gloomEngine),
                       name(name), id(id), parent(parent), tag(tag) {}

GameObject::~GameObject() {}

std::shared_ptr<Component> GameObject::FindComponent(int componentId) {
    if (!components.contains(componentId)) return nullptr;
    return components.find(componentId)->second;
}

std::shared_ptr<Component> GameObject::FindComponentByName(ComponentNames name) {
    for (auto&& component : components) {
        if (component.second->GetName() == name) return component.second;
    }
    return nullptr;
}

void GameObject::AddComponent(std::shared_ptr<Component> &component) {
    components.insert({component->GetId(), component});
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
        child.second->OnTransformUpdateComponents();
        child.second->ForceUpdateSelfAndChildren();
    }
}

int GameObject::GetId() const {
    return id;
}

const std::string &GameObject::GetName() const {
    return name;
}
