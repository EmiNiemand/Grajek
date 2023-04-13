#include "GameObjectsAndPrefabs/GameObject.h"
#include "GloomEngine.h"

GameObject::GameObject(const std::string &name, int id, const std::shared_ptr<GameObject> &parent, Tags tag) :
                                                                        name(name), id(id), parent(parent), tag(tag) {}

GameObject::~GameObject() = default;

std::shared_ptr<GameObject> GameObject::Instantiate(std::string name, std::shared_ptr<GameObject> parent, Tags tag) {
    return GameObjectFactory::GetInstance()->CreateGameObject(name, parent, tag);
}

void GameObject::Destroy(std::shared_ptr<GameObject> gameObject) {
    gameObject->RemoveAllChildren();
    gameObject->RemoveAllComponents();
    GloomEngine::GetInstance()->RemoveGameObject(gameObject);
}

void GameObject::OnTransformUpdateComponents() {
    for (auto&& component : components) {
        component.second->OnUpdate();
    }
}

void GameObject::RemoveComponent(int componentId) {
    if (!components.contains(componentId)) return;
    components.find(componentId)->second->OnDestroy();
    components.erase(componentId);
}

void GameObject::RemoveAllComponents() {
    for (auto&& component : components){
        component.second->OnDestroy();
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
    children.find(childId)->second->RemoveAllComponents();
    GloomEngine::GetInstance()->RemoveGameObject(children.find(childId)->second);
    children.erase(childId);
}

void GameObject::RemoveAllChildren() {
    for (auto&& child : children) {
        child.second->RemoveAllComponents();
        child.second->RemoveAllChildren();
        GloomEngine::GetInstance()->RemoveGameObject(child.second);
    }
    children.clear();
}

void GameObject::UpdateSelfAndChildren() {
    if (transform != nullptr) {
        ForceUpdateSelfAndChildren();
        OnTransformUpdateComponents();
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
