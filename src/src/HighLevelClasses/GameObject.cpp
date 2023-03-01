//
// Created by szymo on 01/03/2023.
//

#include "include/HighLevelClasses/GameObject.h"

GameObject::GameObject(const std::string &name, const Tags &tag, const Transform &transform) : name(name),
                                                               tag(tag), transform(transform) {}

GameObject::~GameObject() {}

void GameObject::AddComponent(std::shared_ptr<Component> component) {
    if (components.contains(component->GetName())) return;
    components.insert({component->GetName(), component});
}

void GameObject::RemoveComponent(ComponentNames componentName) {
    if (!components.contains(componentName)) return;
    components.erase(componentName);
}

void GameObject::AddChild(std::shared_ptr<GameObject> child) {
    child->parent = shared_from_this();
    children.insert({child->GetName(), child});
}

void GameObject::RemoveChild(std::string childName) {
    if (!children.contains(childName)) return;
    children.erase(childName);
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
