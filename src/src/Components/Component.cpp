#include "Components/Component.h"
#include "GloomEngine.h"

Component::Component(const std::shared_ptr<GameObject> &parent,
                     int id) : parent(parent), id(id) {}

Component::~Component() = default;

void Component::Destroy(const std::shared_ptr<Component>& component) {
    GloomEngine::GetInstance()->destroyComponentBuffer.push_back(component);
}

int Component::GetId() const {
    return id;
}

const std::shared_ptr<GameObject> &Component::GetParent() const {
    return parent;
}

