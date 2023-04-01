#include "Components/Component.h"
#include "GloomEngine.h"

Component::Component(const std::shared_ptr<GameObject> &parent,
                     int id) : parent(parent), id(id) {}

Component::~Component() = default;

void Component::OnCreate() {
    GloomEngine::GetInstance()->AddComponent(shared_from_this());
}

void Component::OnDestroy() {
    GloomEngine::GetInstance()->RemoveComponent(shared_from_this());
}

int Component::GetId() const {
    return id;
}

const std::shared_ptr<GameObject> &Component::GetParent() const {
    return parent;
}

void Component::SetParent(const std::shared_ptr<GameObject> &parent) {
    Component::parent = parent;
}

