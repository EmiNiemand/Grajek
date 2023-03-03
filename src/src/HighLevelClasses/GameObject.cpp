//
// Created by szymo on 01/03/2023.
//

#include "include/HighLevelClasses/GameObject.h"
#include "include/GloomEngine.h"
#include "include/Components/Component.h"

GameObject::GameObject(const std::shared_ptr<GloomEngine> &gloomEngine, const std::string &name,
                       const std::shared_ptr<GameObject> &parent, Tags tag) : gloomEngine(gloomEngine),
                       name(name), parent(parent), tag(tag) {}

GameObject::~GameObject() {}

std::shared_ptr<Component> GameObject::FindComponent(ComponentNames componentName) {
    if (!components.contains(componentName)) return nullptr;
    return components.find(componentName)->second;
}

void GameObject::AddComponent(std::shared_ptr<Component> &component) {
    components.insert({component->GetName(), component});
}

void GameObject::OnTransformUpdateComponents() {
    for (auto&& component : components) {
        component.second->OnTransformUpdate();
    }
}

void GameObject::RemoveComponent(ComponentNames componentName) {
    if (!components.contains(componentName)) return;
    components.find(componentName)->second->OnRemove();
    gloomEngine->RemoveComponent(components.find(componentName)->second);
    components.erase(componentName);
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
    children.insert({child->GetName(), child});
}

void GameObject::RemoveChild(std::string childName) {
    if (!children.contains(childName)) return;
    children.find(childName)->second->RemoveAllChildren();
    children.erase(childName);
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

const std::string &GameObject::GetName() const {
    return name;
}
