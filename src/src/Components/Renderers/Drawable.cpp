//
// Created by szymo on 01/04/2023.
//

#include "Components/Renderers/Drawable.h"
#include "EngineManagers/RendererManager.h"

Drawable::Drawable(const std::shared_ptr<GameObject> &parent, int id) : Component(parent, id) {}

Drawable::~Drawable() = default;

void Drawable::Update() {
    AddToDraw();
    Component::Update();
}

void Drawable::AddToDraw() {
    RendererManager::GetInstance()->AddToDrawBuffer(shared_from_this());
}

