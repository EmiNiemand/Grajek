//
// Created by szymo on 01/03/2023.
//

#include "include/Components/Component.h"
#include "include/GloomEngine.h"

Component::Component(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent,
                     int id) : gloomEngine(gloomEngine), parent(parent), id(id) {}

Component::~Component() {}

// TODO: make it call method to update in engine pass id
void Component::OnUpdate() {
    gloomEngine->OnUpdate(id);
}

int Component::GetId() const {
    return id;
}

const ComponentNames &Component::GetName() const {
    return name;
}

const std::shared_ptr<GameObject> &Component::GetParent() const {
    return parent;
}

void Component::SetParent(const std::shared_ptr<GameObject> &parent) {
    Component::parent = parent;
}