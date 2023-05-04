#include <utility>

#include "GameObjectsAndPrefabs/GameObject.h"
#include "GloomEngine.h"
#include "Other/FrustumCulling.h"

GameObject::GameObject(std::string name, uint32_t id, const std::shared_ptr<GameObject> &parent, Tags tag) :
                                                                        name(std::move(name)), id(id), parent(parent), tag(tag) {
    bounds = FrustumCulling::GenerateAABB(nullptr);
}

GameObject::~GameObject() = default;

std::shared_ptr<GameObject> GameObject::Instantiate(std::string name, std::shared_ptr<GameObject> parent, Tags tag) {
    return GameObjectFactory::GetInstance()->CreateGameObject(std::move(name), parent, tag);
}

void GameObject::Destroy(const std::shared_ptr<GameObject>& gameObject) {
    GloomEngine::GetInstance()->destroyGameObjectBuffer.emplace_back(gameObject);
}

void GameObject::OnTransformUpdateComponents() {
    for (auto&& component : components) {
        component.second->OnUpdate();
    }
}

void GameObject::RemoveComponent(uint32_t componentId) {
    if (!components.contains(componentId)) return;
    Component::Destroy(components.find(componentId)->second);
    components.erase(componentId);
}

void GameObject::RemoveAllComponents() {
    for (auto&& component : components) {
        Component::Destroy(component.second);
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

void GameObject::RemoveChild(uint32_t childId) {
    if (!children.contains(childId)) return;
    children.find(childId)->second->RemoveAllChildren();
    children.find(childId)->second->RemoveAllComponents();
    Destroy(children.find(childId)->second);
    children.erase(childId);
}

void GameObject::RemoveAllChildren() {
    for (auto&& child : children) {
        child.second->RemoveAllComponents();
        child.second->RemoveAllChildren();
        Destroy(child.second);
    }
    children.clear();
}

void GameObject::UpdateSelfAndChildren() {
    if (transform != nullptr && dirtyFlag) {
        ForceUpdateSelfAndChildren();
        OnTransformUpdateComponents();
        return;
    }

    for (auto&& child : children) {
        child.second->UpdateSelfAndChildren();
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

void GameObject::EnableSelfAndChildren() {
    if (enabled) return;

    for (auto&& component : components){
        component.second->enabled = true;
    }
    for (auto&& child : children)
    {
        child.second->EnableSelfAndChildren();
    }
    enabled = true;
}

void GameObject::DisableSelfAndChildren() {
    if (!enabled) return;

    for (auto&& child : children)
    {
        child.second->DisableSelfAndChildren();
    }
    for (auto&& component : components){
        component.second->enabled = false;
    }
    enabled = false;
}

int GameObject::GetId() const {
    return id;
}

const std::string &GameObject::GetName() const {
    return name;
}

bool GameObject::GetEnabled() const {
    return enabled;
}

void GameObject::RecalculateGlobalRotation() {
    globalRotation = parent->globalRotation + transform->GetLocalRotation();

    for (auto&& child : children) {
        child.second->RecalculateGlobalRotation();
    }
}
