//
// Created by szymo on 01/03/2023.
//

#include "include/Components/Component.h"

Component::Component(const std::shared_ptr<GloomEngine> &gloomEngine, const std::shared_ptr<GameObject> &parent) : gloomEngine(gloomEngine), parent(parent) {}

Component::~Component() {}

const ComponentNames &Component::GetName() const {
    return name;
}

const std::shared_ptr<GameObject> &Component::GetParent() const {
    return parent;
}

void Component::SetParent(const std::shared_ptr<GameObject> &parent) {
    Component::parent = parent;
}

