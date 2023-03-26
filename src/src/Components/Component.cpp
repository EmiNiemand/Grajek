#include "Components/Component.h"
#include "GloomEngine.h"

Component::Component(const std::shared_ptr<GameObject> &parent,
                     int id) : parent(parent), id(id) {}

Component::~Component() {}

int Component::GetId() const {
    return id;
}

const std::shared_ptr<GameObject> &Component::GetParent() const {
    return parent;
}

void Component::SetParent(const std::shared_ptr<GameObject> &parent) {
    Component::parent = parent;
}