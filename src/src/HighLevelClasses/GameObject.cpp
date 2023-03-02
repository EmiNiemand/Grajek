//
// Created by szymo on 01/03/2023.
//

#include "include/HighLevelClasses/GameObject.h"
#include "include/Components/Component.h"

GameObject::GameObject(const std::shared_ptr<GloomEngine> &gloomEngine, const std::string &name,
                       const std::shared_ptr<GameObject> &parent, Tags tag) : gloomEngine(gloomEngine),
                       name(name), parent(parent), tag(tag) {
    transform = Transform();
}

GameObject::~GameObject() {}

void GameObject::AddComponent(std::shared_ptr<Component> &component) {
    components.insert({component->GetName(), component});
}


void GameObject::RemoveComponent(ComponentNames componentName) {
    if (!components.contains(componentName)) return;
    components.erase(componentName);
}

void GameObject::RemoveAllComponents() {
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
    }
    children.clear();
}

void GameObject::UpdateSelfAndChildren() {
    if (!transform.IsDirty())
        return;

    ForceUpdateSelfAndChildren();
}

void GameObject::ForceUpdateSelfAndChildren() {
    if (parent) transform.ComputeModelMatrix(parent->transform.GetModelMatrix());
    else transform.ComputeModelMatrix();

    for (auto&& child : children)
    {
        child.second->ForceUpdateSelfAndChildren();
    }
}

const std::string &GameObject::GetName() const {
    return name;
}

const std::shared_ptr<Component> &GameObject::FindComponent(ComponentNames componentName) {
    return components.find(componentName)->second;
}